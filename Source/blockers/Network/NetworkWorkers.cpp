// Fill out your copyright notice in the Description page of Project Settings.



#include "NetworkWorkers.h"



int8 recv_th_count = 0;

FRecvWorker::FRecvWorker(FSocket* c_Socket, AblockersCharacter* Character) : socket(c_Socket), Character(Character)
{
   
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

    th_num = recv_th_count++;

    while (recvRunning) {
        // 버퍼에 읽어올 데이터가 있는지 확인
        uint32 bHasPendingData = 0;
        socket->HasPendingData(bHasPendingData);

        if (bHasPendingData > 0) {

            int32 BytesRead = 0;
            socket->Recv(recv_buf, sizeof(recv_buf), BytesRead);
            if (recv_buf[1] != SC_MOVE_PLAYER)
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%dRecv_Thread Check Packet Type: %d"), th_num, recv_buf[1]));
            switch (recv_buf[1]){
                
            case SC_LOGIN_INFO: {
                SC_LOGIN_INFO_PACKET info;

                memcpy(&info, recv_buf, sizeof(SC_LOGIN_INFO_PACKET));
                FVector NewLocation(info.x, info.y, info.z);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%dRecv_Thread Recv Login Packet id: %d, x: %f, y: %f, z: %f"), th_num, info.id, info.x, info.y, info.z));

                AsyncTask(ENamedThreads::GameThread, [this, NewLocation, info]()
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Login Packet")));
                        if (IsValid(Character)) {
                            for (auto& p : Character->Players) {
                                if (p->id < 0) {
                                    p->id = info.id;
                                    Character->id = info.id;
                                    p->SetActorLocation(NewLocation);
                                    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Set Login Info id: %d, x: %f, y: %f, z: %f"), info.id, info.x, info.y, info.z));
                                    break;
                                }
                            }
                        }
                        else
                            recvRunning = false;
                    });
                Character->loginOk = true;

                break;
            }
               
            case SC_MOVE_PLAYER:{
                SC_MOVE_PLAYER_PACKET new_pos;

                memcpy(&new_pos, recv_buf, sizeof(new_pos));
                FVector NewLocation(new_pos.x, new_pos.y, new_pos.z);
                FRotator NewRotation(new_pos.pitch, new_pos.yaw, new_pos.roll);
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Move Packet id: %d, x: %f, y: %f, z: %f"), new_pos.id, new_pos.x, new_pos.y, new_pos.z));

                // AsyncTask를 사용하여 메인 스레드에서 캐릭터의 위치 업데이트
                AsyncTask(ENamedThreads::GameThread, [this, NewLocation, NewRotation, new_pos]()
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Move Packet id: %d, x: %f, y: %f, z: %f"), new_pos.id, new_pos.x, new_pos.y, new_pos.z));
                        if (IsValid(Character))
                        {
                            for (auto& p : Character->Players) {
                                if (p->id == new_pos.id && Character->id != new_pos.id) {
                                    p->SetActorLocation(NewLocation);
                                    p->SetActorRotation(NewRotation);
                                    break;
                                }
                            }
                        }
                        else
                            recvRunning = false;
                    });
                break;
             
            }
            case SC_ADD_PLAYER: {
                SC_ADD_PLAYER_PACKET new_player;

                memcpy(&new_player, recv_buf, sizeof(new_player));
                FVector NewLocation(new_player.x, new_player.y, new_player.z);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%dRecv_Thread Recv Add Packet id: %d, x: %f, y: %f, z: %f"), th_num, new_player.id, new_player.x, new_player.y, new_player.z));

                AsyncTask(ENamedThreads::GameThread, [this, NewLocation, new_player]()
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Move Packet")));
                        if (IsValid(Character)){   
                            for (auto& p : Character->Players) {
                                if (p->id < 0) {
                                    p->id = new_player.id;
                                    p->SetActorLocation(NewLocation);
                                    break;
                                }
                            }
                           
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
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SendWorker Running Start")));
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

    while (!Character->loginOk) {
        // 로그인 패킷 받기 전까지 대기
    }
       
    double LastSendTime = FPlatformTime::Seconds();

    while (sendRunning)
    {
        if (IsValid(Character))
        {
            CurrentLocation = Character->GetActorLocation();
            CurrentRotation = Character->GetActorRotation();

            double CurrentTime = FPlatformTime::Seconds();

            if (CurrentTime - LastSendTime >= 0.03)
            {
                // 위치 및 회전 정보 패킷 구성
                new_pos.type = CS_MOVE;
                new_pos.size = sizeof(CS_MOVE_PACKET);
                new_pos.x = CurrentLocation.X;
                new_pos.y = CurrentLocation.Y;
                new_pos.z = CurrentLocation.Z;
                new_pos.pitch = CurrentRotation.Pitch;
                new_pos.yaw = CurrentRotation.Yaw;
                new_pos.roll = CurrentRotation.Roll;

                BytesSent = 0;
                socket->Send((uint8*)&new_pos, sizeof(new_pos), BytesSent);

                LastSendTime = CurrentTime;

                // 디버그 메시지 출력
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Send My Move Packet, x: %f, y: %f, z: %f"), new_pos.x, new_pos.y, new_pos.z));
            }
        }
        else
        {
            sendRunning = false;
        }

        //FPlatformProcess::Sleep(0.01);
    }

    return 0;
}

void FSendWorker::Stop()
{
    sendRunning = false;
}

