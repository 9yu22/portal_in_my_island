// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "ItemSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnableItem {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<APickUpItem> ItemToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnSecond;

	float CurrTime;
};

UCLASS()
class AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnItem(const int32 index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<FSpawnableItem> ItemArray;

private:
	
};
