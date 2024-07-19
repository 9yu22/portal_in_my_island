// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersGameMode.h"
#include "BKPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Misc/Memory.h"
#include "Network/Protocol.h"


AblockersGameMode::AblockersGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABKPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blockers/Blueprints/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blockers/Blueprints/Game/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// Set default HUD class to our Blueprinted HUD
	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/Blockers/Blueprints/Game/BlockersHUD"));
	if (HUDBPClass.Succeeded())
	{
		// Assign the found HUD class
		HUDClass = HUDBPClass.Class;
	}
}

void AblockersGameMode::SpawnCharacter(SC_ADD_PLAYER_PACKET new_player)
{
	if (UWorld* World = GetWorld())
	{
		FVector SpawnLocation(new_player.x, new_player.y, new_player.z);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// 클래스 이름으로 클래스를 동적으로 로드
		UClass* AblockersCharacterClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blockers/Blueprints/BP_Player.BP_Player_C"));

		if (AblockersCharacterClass)
		{
			// CharacterClass를 사용하여 캐릭터를 스폰
			AblockersCharacter* SpawnCharacter = World->SpawnActor<AblockersCharacter>(AblockersCharacterClass, SpawnLocation, SpawnRotation);

			USGameInstance* instance = USGameInstance::GetMyInstance(this);

			if (SpawnCharacter && instance)
			{
				SpawnCharacter->SetTextureForCharacter(new_player.id);

				SpawnCharacter->id = new_player.id;
				UCharacterMovementComponent* MovementComponent = SpawnCharacter->GetCharacterMovement();
				MovementComponent->GravityScale = 0.0f;  // 중력을 0으로 설정				
				instance->Players.Add(SpawnCharacter);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Client id:%d / Spawned Character id: %d, x: %f, y: %f, z: %f"), instance->MyCharacter->id, SpawnCharacter->id,
					SpawnCharacter->GetActorLocation().X, SpawnCharacter->GetActorLocation().Y, SpawnCharacter->GetActorLocation().Z));
			}
		}
	}
}

void AblockersGameMode::SpawnPortal(SC_ADD_PORTAL_PACKET add_portal)
{
	if (UWorld* World = GetWorld())
	{
		FVector SpawnLocation(add_portal.x, add_portal.y, add_portal.z);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// 클래스 이름으로 클래스를 동적으로 로드
		UClass* PortalClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blockers/Blueprints/BP_Portal.BP_Portal_C"));		

		if (PortalClass)
		{
			// PortalClass를 사용하여 포탈을 스폰
			APortal* SpawnPortal = World->SpawnActor<APortal>(PortalClass, SpawnLocation, SpawnRotation);

			USGameInstance* instance = USGameInstance::GetMyInstance(this);

			if (SpawnPortal && instance)
			{
				SpawnPortal->id = add_portal.id;
				SpawnPortal->Portalhealth = add_portal.hp;
				instance->Portals.Add(SpawnPortal);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Spawned Portal id: %d, x: %f, y: %f, z: %f / hp: %f"), SpawnPortal->id,
					SpawnPortal->GetActorLocation().X, SpawnPortal->GetActorLocation().Y, SpawnPortal->GetActorLocation().Z, SpawnPortal->Portalhealth));
			}
		}
	}
}



void AblockersGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Spawn 'i' players
	//TSet<ABKPlayerController*> PlayerControllersSet;
	//for (int32 i = 0; i < 1; i++)
	//{
	//	// Spawn player
	//	ABKPlayerController* NewPlayerController = Cast<ABKPlayerController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));

	//	if (NewPlayerController)
	//	{

	//		PlayerControllersSet.Add(NewPlayerController);
	//	}
	//}

	//Bind our Player died delegate to the Gamemode's PlayerDied function.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AblockersGameMode::PlayerDied);
	}


}

void AblockersGameMode::PlayerDied(ACharacter* Character)
{
	AController* CharacterController = Character -> GetController();
}
