// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpItem.h"
#include "ResourceItem.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKERS_API AResourceItem : public APickUpItem
{
	GENERATED_BODY()

public:
	int id = -1;
	
protected:
	virtual void UseItem() override;
};
