// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Protocol.h"

USGameInstance::USGameInstance()
{
	Socket = nullptr;
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

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server..")));

	bool Connected = Socket->Connect(*InternetAddr); // 블로킹 방식, 연결 부분은 굳이 논블로킹 방식 고려 필요 x(체감 잘 안됨)

	if (Connected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Connection Success!")));

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
			FRecvWorker* RecvWorker = new FRecvWorker(this);
			//FSendWorker* SendWorker = new FSendWorker(this, Character);
			// 스레드 생성 및 시작
			RecvThread = FRunnableThread::Create(RecvWorker, TEXT("RecvWorkerThread"), 0, TPri_Normal);
			//SendThread = FRunnableThread::Create(SendWorker, TEXT("SendWorkerThread"), 0, TPri_Normal);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
		Socket = nullptr;
	}
}

void USGameInstance::DisconnectFromGameServer()
{
	if (IsValid(MyCharacter)) {
		CS_DISCONNECT_PACKET dis;
		dis.size = sizeof(dis);
		dis.type = CS_DISCONNECT;
		dis.id = MyCharacter->id;

		int bytesSent = 0;
		Socket->Send((uint8*)&dis, sizeof(dis), bytesSent);
	}
	
	if (RecvThread)
	{
		RecvThread->Kill(true);
		delete RecvThread;
		RecvThread = nullptr;
	}

	if (SendThread)
	{
		SendThread->Kill(true);
		delete SendThread;
		SendThread = nullptr;
	}

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

void USGameInstance::SpawnCharacter(SC_ADD_PLAYER_PACKET* new_player)
{
	// 게임 스레드에서 실행되도록 AsyncTask 사용
	AsyncTask(ENamedThreads::GameThread, [this, new_player]()
		{
			if (UWorld* World = GetWorld())
			{
				FVector SpawnLocation = FVector(new_player->x, new_player->y, new_player->z);
				FRotator SpawnRotation = FRotator::ZeroRotator;

				// 클래스 이름으로 클래스를 동적으로 로드
				UClass* AblockersCharacterClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blockers/Blueprints/BP_Player.BP_Player_C"));

				if (AblockersCharacterClass)
				{
					// CharacterClass를 사용하여 캐릭터를 스폰
					AActor* SpawnActor = World->SpawnActor<AActor>(AblockersCharacterClass, SpawnLocation, SpawnRotation);

					if (SpawnActor)
					{
						// AActor 포인터를 AblockersCharacter 포인터로 캐스팅
						AblockersCharacter* SpawnCharacter = Cast<AblockersCharacter>(SpawnActor);
						if (SpawnCharacter)
						{
							Players.Add(SpawnCharacter);
							SpawnCharacter->id = new_player->id;
							UCharacterMovementComponent* MovementComponent = SpawnCharacter->GetCharacterMovement();
							MovementComponent->GravityScale = 0.0f;  // 중력을 0으로 설정
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Add Packet id: %d, x: %f, y: %f, z: %f"), SpawnCharacter->id,
								SpawnCharacter->GetActorLocation().X, SpawnCharacter->GetActorLocation().Y, SpawnCharacter->GetActorLocation().Z));
						}
					}					
				}
			}
		});
}

void USGameInstance::Shutdown()
{
	// 부모 클래스의 Shutdown 호출
	Super::Shutdown();

	// 게임 종료 시 서버 연결 해제
	DisconnectFromGameServer();
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
