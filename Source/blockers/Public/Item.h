// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../blockersCharacter.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class BLOCKERS_API UItem : public UObject
{
	GENERATED_BODY()
	
public:
	UItem();

	virtual class UWorld* GetWorld() const { return World; };

	UPROPERTY(Transient)
	class UWorld* World;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item"); //Text for using the item 
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item"); //Mesh to display for this item
	class UStaticMesh* PickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item"); //Thumbnail for this item
	class UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item"); //Display name for this item in the inventory
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true)); //An optional description for the item
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0)); //Weight of the Item
	float Weight;

	UPROPERTY() //Inventory that owns this item
	class UInventoryComponent* OwningInventory; 

	virtual void Use(class AblockersCharacter* character) PURE_VIRTUAL(UItem, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class AblockersCharacter* Character);
};
