// Fill out your copyright notice in the Description page of Project Settings.

#define TEST 100
#define LOCATION 90

#include "NetworkWorkers.h"

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
    UPacketHeader header;
    LocationPacket locationPacket;

    //TestPacket t;
    //int32 tempread = 0;
    //socket->Recv((uint8*)&t, sizeof(TestPacket), tempread);

    while (recvRunning)
    {
        // 버퍼에 읽어올 데이터가 있는지 확인
        uint32 bHasPendingData = 0;
        socket->HasPendingData(bHasPendingData);

        if (bHasPendingData > 0)
        {
            int32 BytesRead = 0;
            if (socket->Recv((uint8*)&header, sizeof(UPacketHeader), BytesRead))
            {
                switch (header.type)
                {
                case LOCATION:
                {
                    if (socket->Recv(((uint8*)&locationPacket) + sizeof(UPacketHeader), sizeof(locationPacket) - sizeof(UPacketHeader), BytesRead))
                    {
                        FVector NewLocation(locationPacket.x, locationPacket.y, locationPacket.z);

                        // AsyncTask를 사용하여 메인 스레드에서 캐릭터의 위치 업데이트
                        AsyncTask(ENamedThreads::GameThread, [this, NewLocation]()
                            {
                                if (IsValid(Character))
                                {
                                    Character->SetActorLocation(NewLocation);
                                }
                                else
                                    recvRunning = false;
                            });
                    }
                    break;
                }
                case TEST:
                    TestPacket test;
                    if (socket->Recv(((uint8*)&test) + sizeof(UPacketHeader), sizeof(TestPacket) - sizeof(UPacketHeader), BytesRead))
                    {

                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Test Packet Recv: %d"), test.number));
                    }
                }
            }
        }

        //// 프로세스를 매우 빠르게 반복하지 않도록 약간의 지연을 추가
        //FPlatformProcess::Sleep(0.01);
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
    //UPacketHeader Header;
    LocationPacket location;
    FVector lastLocation;

    while (sendRunning)
    {
        // 캐릭터 위치 가져오기
        CurrentLocation = Character->GetActorLocation();

        if (!CurrentLocation.Equals(lastLocation, KINDA_SMALL_NUMBER))
        {
            if (Character)
            {
                // 위치 정보 패킷 구성
                location.type = LOCATION;
                location.size = sizeof(LocationPacket);
                location.x = CurrentLocation.X;
                location.y = CurrentLocation.Y;
                location.z = CurrentLocation.Z;

                // 패킷 전송
                int32 BytesSent = 0;
                socket->Send((uint8*)&location, sizeof(LocationPacket), BytesSent);
                lastLocation = CurrentLocation;
            }

            FPlatformProcess::Sleep(0.01);
        }
    }

    return 0;
}

void FSendWorker::Stop()
{
    sendRunning = false;
}

