// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkWorkers.h"
//#include "S1.h"
#include "PacketSession.h"
#include "../blockersCharacter.h"
#include "SGameInstance.generated.h"

/**
 *
 */
UCLASS()
class BLOCKERS_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	//UFUNCTION(BlueprintCallable)
	//void HandleRecvPackets();

	//void SendPacket(SendBufferRef SendBuffer);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 4040;
	AblockersCharacter* Character;

	//TSharedPtr<class PacketSession> GameServerSession;
	//TSharedPtr<class FRecvWorker> makeRecvThread;
};
