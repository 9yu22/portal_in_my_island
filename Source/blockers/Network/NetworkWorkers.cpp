// Fill out your copyright notice in the Description page of Project Settings.



#include "NetworkWorkers.h"

std::mutex getPosMx;
bool loginOk = false;

FRecvWorker::FRecvWorker(FSocket* c_Socket, AblockersCharacter* Character) : socket(c_Socket), Character(Character)
{
    //recvThread = FRunnableThread::Create(this, TEXT("recvThread"));
}

FRecvWorker::~FRecvWorker()
{

}

void FRecvWorker::RecvPacket()
{

}

bool FRecvWorker::Init()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
    return true;
}

uint32 FRecvWorker::Run()
{
    uint8 recv_buf[256];

    while (recvRunning) {
        // ���ۿ� �о�� �����Ͱ� �ִ��� Ȯ��
        uint32 bHasPendingData = 0;
        socket->HasPendingData(bHasPendingData);

        if (bHasPendingData > 0) {

            int32 BytesRead = 0;
            socket->Recv(recv_buf, sizeof(recv_buf), BytesRead);
            switch (recv_buf[1]){

            case SC_LOGIN_INFO: {
                SC_LOGIN_INFO_PACKET info;
                if (BytesRead < sizeof(info))
                    break;
                memcpy(&info, recv_buf, sizeof(SC_LOGIN_INFO_PACKET));
                FVector NewLocation(info.x, info.y, info.z);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Login Packet id: %f, x: %f, y: %f, z: %f"), info.id, info.x, info.y, info.z));

                AsyncTask(ENamedThreads::GameThread, [this, NewLocation]()
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Login Packet")));
                        if (IsValid(Character))
                        {
                            getPosMx.lock();
                            Character->SetActorLocation(NewLocation);
                            getPosMx.unlock();
                        }
                        else
                            recvRunning = false;
                    });
                loginOk = true;
                break;
            }
               
            case SC_MOVE_PLAYER:{
                SC_MOVE_PLAYER_PACKET new_pos;
                if (BytesRead < sizeof(new_pos)) // �ϴ��� ������ ��Ŷ ũ�Ⱑ ���� ũ�⺸�� �۴ٸ� ó�� ����.->���߿� �������� ����
                    break;
                memcpy(&new_pos, recv_buf, sizeof(new_pos));
                FVector NewLocation(new_pos.x, new_pos.y, new_pos.z);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Move Packet x: %f, y: %f, z: %f"), new_pos.x, new_pos.y, new_pos.z));

                // AsyncTask�� ����Ͽ� ���� �����忡�� ĳ������ ��ġ ������Ʈ
                AsyncTask(ENamedThreads::GameThread, [this, NewLocation]()
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Move Packet")));
                        if (IsValid(Character))
                        {
                            getPosMx.lock();
                            Character->SetActorLocation(NewLocation);
                            getPosMx.unlock();
                        }
                        else
                            recvRunning = false;
                    });
                break;
            }
            case TEST: {
                TestPacket test;
                memcpy(&test, recv_buf, sizeof(TestPacket));

                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Test Packet Recv: %d"), test.number));

            }
               
            }
        }

        FPlatformProcess::Sleep(0.01);
    }
    return 0;
}

void FRecvWorker::Stop()
{
    recvRunning = false;
}

// SendWorker

FSendWorker::FSendWorker(FSocket* c_Socket, AblockersCharacter* Character) : socket(c_Socket), Character(Character)
{
    
}

FSendWorker::~FSendWorker()
{
}

bool FSendWorker::Init()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));
    return true;
}

uint32 FSendWorker::Run()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SendWorker Running Start")));
    //UPacketHeader Header;
    CS_MOVE_PACKET new_pos;
    FVector lastLocation;

    CS_LOGIN_PACKET login;
    login.size = sizeof(CS_LOGIN_PACKET);
    login.type = CS_LOGIN;

    int32 BytesSent = 0;
    socket->Send((uint8*)&login, sizeof(login), BytesSent);
    if (BytesSent > 0)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Packet Send")));
    else
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Packet Send Fail...")));

    while (!loginOk)
        // �α��� ��Ŷ �ޱ� ������ ���

    while (sendRunning)
    {
        // ĳ���� ��ġ ��������
        getPosMx.lock();
        CurrentLocation = Character->GetActorLocation();
        getPosMx.unlock();
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Get Player Pos, x: %f, y: %f, z: %f"), CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z));

        if (IsValid(Character))
        {
            if (!CurrentLocation.Equals(lastLocation, KINDA_SMALL_NUMBER))
            {
                // ��ġ ���� ��Ŷ ����
                new_pos.type = CS_MOVE;
                new_pos.size = sizeof(CS_MOVE_PACKET);
                new_pos.x = CurrentLocation.X;
                new_pos.y = CurrentLocation.Y;
                new_pos.z = CurrentLocation.Z;

                // ��Ŷ ����
                BytesSent = 0;
                socket->Send((uint8*)&new_pos, sizeof(new_pos), BytesSent);
                lastLocation = CurrentLocation;
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Move Packet, x: %f, y: %f, z: %f"), new_pos.x, new_pos.y, new_pos.z));
            }        
        }
        else
            sendRunning = false;
        FPlatformProcess::Sleep(0.01);
    }

    return 0;
}

void FSendWorker::Stop()
{
    sendRunning = false;
}
