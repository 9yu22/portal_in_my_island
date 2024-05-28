// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Protocol.h"

USGameInstance::USGameInstance()
{
}

void USGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	bool ret = SetIpAddress();
	if (!ret)
		return;

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server..")));

	bool Connected = Socket->Connect(*InternetAddr); // 블로킹 방식, 연결 부분은 굳이 논블로킹 방식 고려 필요 x(체감 잘 안됨)

	if (Connected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success!")));

		// Session
		// PacketSession 객체 생성 및 GameServerSession 에 TSharedPtr객체 할당, 이 객체에 생성된 객체를 가리키는 포인터 존재
		/*GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();*/

		CS_LOGIN_PACKET login;
		login.size = sizeof(CS_LOGIN_PACKET);
		login.type = CS_LOGIN;

		int32 BytesSent = 0;
		Socket->Send((uint8*)&login, sizeof(login), BytesSent);
		if (BytesSent > 0)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Login Packet Send")));
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Packet Send Fail...")));

		AblockersCharacter* Character = Cast<AblockersCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (Character) {
			MyCharacter = Character;
			FRecvWorker* RecvWorker = new FRecvWorker(this, Character);
			FSendWorker* SendWorker = new FSendWorker(this, Character);
			// 스레드 생성 및 시작
			FRunnableThread* RecvThread = FRunnableThread::Create(RecvWorker, TEXT("RecvWorkerThread"), 0, TPri_Normal);
			FRunnableThread* SendThread = FRunnableThread::Create(SendWorker, TEXT("SendWorkerThread"), 0, TPri_Normal);
		}
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

USGameInstance* USGameInstance::GetMyInstance(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		return Cast<USGameInstance>(World->GetGameInstance());
	}

	return nullptr;
}

bool USGameInstance::SetIpAddress()
{
	FString IpAddressPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("IpAddress.txt"));

	// 파일을 읽어 IpAddress 문자열에 저장
	if (FFileHelper::LoadFileToString(IpAddress, *IpAddressPath))
		return true;

	return false;
}


//void US1GameInstance::HandleRecvPackets()
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	// 워커스레드가 큐에 쌓아놓은 것을 소모
//	GameServerSession->HandleRecvPackets();
//}
//
//void US1GameInstance::SendPacket(SendBufferRef SendBuffer)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	// 큐에 패킷을 쌓는 역할
//	GameServerSession->SendPacket(SendBuffer);
//}
