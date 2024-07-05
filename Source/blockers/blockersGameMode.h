// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Controller.h"
#include "blockersCharacter.h"
#include "blockersGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

UCLASS(minimalapi)
class AblockersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AblockersGameMode();

	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

protected:
	virtual void BeginPlay()override;

	UFUNCTION()
	virtual void PlayerDied(ACharacter* Character); //Call When Player die
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied; //Signature to bind delegate

};



