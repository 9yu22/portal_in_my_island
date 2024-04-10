// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersGameMode.h"
#include "BKPlayerController.h"
#include "UObject/ConstructorHelpers.h"


AblockersGameMode::AblockersGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABKPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blockers/Blueprints/BP_Player")); //BP_PlayerPawn 이용시 총알 발사.
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
