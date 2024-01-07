// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersGameMode.h"
#include "blockersCharacter.h"
#include "UObject/ConstructorHelpers.h"

AblockersGameMode::AblockersGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
