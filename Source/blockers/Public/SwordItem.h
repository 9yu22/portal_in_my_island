// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpItem.h"
#include "SwordItem.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKERS_API ASwordItem : public APickUpItem
{
	GENERATED_BODY()
	
protected:
	virtual void UseItem() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;
};
