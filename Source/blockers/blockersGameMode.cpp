// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersGameMode.h"
#include "BKPlayerController.h"
#include "blockersCharacter.h"
#include "UObject/ConstructorHelpers.h"

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
