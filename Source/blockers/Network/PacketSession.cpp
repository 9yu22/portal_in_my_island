// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketSession.h"


PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	// AsShared()는 Weak포인터를 Shared포인터로 바꿔줌
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
}

void PacketSession::Recv()
{
}

void PacketSession::Disconnect()
{
}

void PacketSession::HandleRecvPackets()
{
	while (true) {
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		// Switch로 받게되면 수가 너무 많아져 관리가 힘들다.
		// TODO
		// ClientPacketHandler::HandlePacket();
	}
}
