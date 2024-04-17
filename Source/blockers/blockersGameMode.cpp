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


	// Spawn 3 players
	for (int32 i = 0; i < 3; i++)
	{
		// Spawn player
		APlayerController* NewPlayerController = Cast<APlayerController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));

		if (NewPlayerController)
		{
			// Optionally, you can set initial properties or perform other setup for each player here

			// You might want to store references to these player controllers for further manipulation
			PlayerControllersArray.Add(NewPlayerController);
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
	//Get a reference to our Character's Player Controller
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}
