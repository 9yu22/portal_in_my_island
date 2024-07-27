// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "../blockersCharacter.h"
#include "GameFramework/Character.h"
#include "SGameInstance.h"
//#include "../Private/BKChunkBase.h"
#include "Protocol.h"
#include <mutex>
/**
*
*/

class USGameInstance;

class BLOCKERS_API FRecvWorker : public FRunnable
{
private:
    USGameInstance* Instance;
    //AblockersCharacter* Character;
    bool recvRunning = true;
    //int8 th_num = 0; // µð¹ö±ë¿ë

public:
    FRecvWorker(USGameInstance* Instance);
    virtual ~FRecvWorker();

    void RecvPacket();
    void MergePacket(uint8* buffer, int32 recvPacketSize);
    void ProcessPacket(uint8* packet);

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
};

//class BLOCKERS_API FSendWorker : public FRunnable
//{
//private:
//    USGameInstance* Instance;
//    FSocket* c_Socket;
//    AblockersCharacter* Character;
//    bool sendRunning = true;
//
//    FVector CurrentLocation;
//    FRotator CurrentRotation;
//
//public:
//    FSendWorker(USGameInstance* Instance, AblockersCharacter* Character);
//    virtual ~FSendWorker();
//
//    virtual bool Init() override;
//    virtual uint32 Run() override;
//    virtual void Stop() override;
//
//    //void UpdateLocation(const FVector& NewLocation);
//};