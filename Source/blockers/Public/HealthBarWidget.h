// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../blockersCharacter.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKERS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetOwnerCharacter(AblockersCharacter* InCharacter) { OwnerCharacter = InCharacter; }

protected:

	AblockersCharacter* OwnerCharacter;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthLabel;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
