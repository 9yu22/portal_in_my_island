// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;

struct BLOCKERS_API FPacketHeader
{
	uint16 PacketSize;
	uint16 PacketID;

	FPacketHeader() : PacketSize(0), PacketID(0)
	{
	}

	FPacketHeader(uint16 InPacketSize, uint16 InPacketID)
		: PacketSize(InPacketSize), PacketID(InPacketID)
	{
	}

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketID;
		return Ar;
	}
};

class BLOCKERS_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~RecvWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;
};
