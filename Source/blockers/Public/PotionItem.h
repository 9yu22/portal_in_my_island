// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PotionItem.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKERS_API UPotionItem : public UItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0)); //Weight of the Item
	float HealthToHeal;
	
protected:
	virtual void Use(class AblockersCharacter* Character) override;
};
