// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "../blockersCharacter.h"
#include "GameFramework/Character.h"
#include "Protocol.h"
#include <mutex>
/**
*
*/

class BLOCKERS_API FRecvWorker : public FRunnable
{
private:
    FSocket* socket;
    AblockersCharacter* Character;
    bool recvRunning = true;

public:
    FRecvWorker(FSocket* c_Socket, AblockersCharacter* Character);
    virtual ~FRecvWorker();

    void RecvPacket();

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
};

class BLOCKERS_API FSendWorker : public FRunnable
{
private:
    FSocket* socket;
    AblockersCharacter* Character;
    bool sendRunning = true;

    FVector CurrentLocation;

public:
    FSendWorker(FSocket* c_Socket, AblockersCharacter* Character);
    virtual ~FSendWorker();

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;

    //void UpdateLocation(const FVector& NewLocation);
};