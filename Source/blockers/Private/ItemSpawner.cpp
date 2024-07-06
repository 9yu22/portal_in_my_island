// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < ItemArray.Num(); ++i)
	{
		ItemArray[i].CurrTime += DeltaTime;

		if (ItemArray[i].CurrTime >= ItemArray[i].SpawnSecond)
		{
			SpawnItem(i);

			ItemArray[i].CurrTime -= ItemArray[i].SpawnSecond;
		}
	}
}

void AItemSpawner::SpawnItem(const int32 index)
{
	if (ItemArray[index].ItemToSpawn)
	{
		// Get the current location and rotation of the spawner
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		// Spawn the actor
		APickUpItem* SpawnedActor = GetWorld()->SpawnActor<APickUpItem>(ItemArray[index].ItemToSpawn, SpawnLocation, SpawnRotation);

		//UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s, SpawnRotation: %s"), *SpawnLocation.ToString(), *SpawnRotation.ToString());

		if (SpawnedActor)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Spawn"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("No Spawn"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemToSpawn is not set!"));
	}
}

