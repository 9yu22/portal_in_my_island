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

	bool SetIpAddress();

public:
	class FSocket* Socket;
	//FString IpAddress = TEXT("127.0.0.1");
	FString IpAddress;
	int16 Port = 4040;

	//TSharedPtr<class PacketSession> GameServerSession;
	//TSharedPtr<class FRecvWorker> makeRecvThread;
};

//TArray<AblockersCharacter*> players; //전역으로 해서 변경된 사항을 항상 가져올 수 있게 해보자