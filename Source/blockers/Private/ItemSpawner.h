// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "ItemSpawner.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnItem();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<APickUpItem> ItemToSpawn;

private:
	float CurrTime;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;
};
