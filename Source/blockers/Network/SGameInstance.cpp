// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Kismet/GameplayStatics.h"


void USGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server..")));

	bool Connected = Socket->Connect(*InternetAddr); // ����ŷ ���, ���� �κ��� ���� ������ŷ ��� ���� �ʿ� x(ü�� �� �ȵ�)

	if (Connected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success!")));

		// Session
		// PacketSession ��ü ���� �� GameServerSession �� TSharedPtr��ü �Ҵ�, �� ��ü�� ������ ��ü�� ����Ű�� ������ ����
		/*GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();*/

		Character = Cast<AblockersCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		FRecvWorker* RecvWorker = new FRecvWorker(Socket, Character);
		FSendWorker* SendWorker = new FSendWorker(Socket, Character);
		// ������ ���� �� ����
		FRunnableThread* RecvThread = FRunnableThread::Create(RecvWorker, TEXT("RecvWorkerThread"), 0, TPri_BelowNormal);
		FRunnableThread* SendThread = FRunnableThread::Create(SendWorker, TEXT("SendWorkerThread"), 0, TPri_BelowNormal);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void USGameInstance::DisconnectFromGameServer()
{
	if (Socket) {
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

//void US1GameInstance::HandleRecvPackets()
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	// ��Ŀ�����尡 ť�� �׾Ƴ��� ���� �Ҹ�
//	GameServerSession->HandleRecvPackets();
//}
//
//void US1GameInstance::SendPacket(SendBufferRef SendBuffer)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	// ť�� ��Ŷ�� �״� ����
//	GameServerSession->SendPacket(SendBuffer);
//}