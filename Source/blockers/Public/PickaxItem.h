// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpItem.h"
#include "PickaxItem.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKERS_API APickaxItem : public APickUpItem
{
	GENERATED_BODY()
	
protected:
	virtual void UseItem() override;
	virtual float GetBlockDamage() override;
	virtual float GetPlayerDamage() override;

public:
	int8 GetPickaxType(APickaxItem* MyActor) const;
};
