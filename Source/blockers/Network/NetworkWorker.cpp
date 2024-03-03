// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkWorker.h"
#include "Sockets.h"
#include "Serialization/ArrayWriter.h"
#include "PacketSession.h"

RecvWorker::RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvWorker::~RecvWorker()
{
}

bool RecvWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	return true;
}

uint32 RecvWorker::Run() // 패킷 조립
{
	while (Running) {
		TArray<uint8> Packet;

		// 뱓은 패킷을 큐에 넣어놓고 메인 스레드에서 꺼내가게 만들자.
		if (ReceivePacket(OUT Packet)) {

			// WeakPtr은 바로 사용 불가->SharedPtr로 변환 필요(lock->언리얼은 pin)
			if (TSharedPtr<PacketSession> Session = SessionRef.Pin()) {
				Session->RecvPacketQueue.Enqueue(Packet);
			}

		}
	}

	return 0;
}

void RecvWorker::Exit()
{
}

// Run 정지
void RecvWorker::Destroy()
{
	Running = false;
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OutPacket)
{
	// 일단 헤더 크기만큼 받을 수 있는지 확인
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID: %d, PacketSize: %d"), Header.PacketID, Header.PacketSize)
	}

	// 패킷 헤더 복사
	OutPacket = HeaderBuffer;

	TArray<uint8> PayloadBuffer;
	// 패킷 크기 = 전체 크기 - 헤더 크기
	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	// 헤더 뒤에 데이터만큼 메모리 추가 할당
	OutPacket.AddZeroed(PayloadSize);

	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;

	return false;
}

// 원하는 크기의 데이터가 들어올 때 까지 대기
bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	uint32 PendingDAtaSIze;
	// 보통 접속 종료 시 패킷 리시브 크기가 0으로 온다.
	if (Socket->HasPendingData({ PendingDAtaSIze }) == false || PendingDAtaSIze <= 0) {
		return false;
	}
	int32 Offset = 0;

	while (Size > 0) {
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;
		Size -= NumRead;
	}
	return false;
}

// --------SendWorker--------

SendWorker::SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

SendWorker::~SendWorker()
{

}

bool SendWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));
	return true;
}

uint32 SendWorker::Run()
{
	while (Running) {
		SendBufferRef SendBuffer;

		if (TSharedPtr<PacketSession> Session = SessionRef.Pin()) {
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer)) {
				SendPacket(SendBuffer);
			}
		}

		//Sleep?
	}

	return 0;
}

void SendWorker::Exit()
{
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	Running = false;
}

bool SendWorker::SendDesiredBytes(uint8* Buffer, int32 Size)
{
	while (Size > 0) {
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == 0)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}
