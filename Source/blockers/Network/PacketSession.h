// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkWorker.h"

/**
 * 
 */
class BLOCKERS_API PacketSession : public TSharedFromThis<PacketSession> // TSharedPtr(스마트 포인터)
{
public:
	PacketSession(class FSocket* Sokcet);
	~PacketSession();

	void Run();
	void Recv();
	void Disconnect();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	//UFUNCTION(BlueprintCallable)
	//void SendOnePacket(TCHAR * buf, int16 size);

public:
	class FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;

	// GameThread와 NetworkThread가 데이터를 주고 받는 큐
	TQueue<TArray<uint8>> RecvPacketQueue;
};
