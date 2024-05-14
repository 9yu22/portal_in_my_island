// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersGameMode.h"
#include "BKPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


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
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blockers/Blueprints/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

}

void AblockersGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Spawn 'i' players
	TSet<ABKPlayerController*> PlayerControllersSet;
	for (int32 i = 0; i < 1; i++)
	{
		// Spawn player
		ABKPlayerController* NewPlayerController = Cast<ABKPlayerController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));

		if (NewPlayerController)
		{

			PlayerControllersSet.Add(NewPlayerController);
		}
	}

	//Bind our Player died delegate to the Gamemode's PlayerDied function.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AblockersGameMode::PlayerDied);
	}


}



void AblockersGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

}

void AblockersGameMode::PlayerDied(ACharacter* Character)
{
	AController* CharacterController = Character -> GetController();
	RestartPlayer(CharacterController);
}
