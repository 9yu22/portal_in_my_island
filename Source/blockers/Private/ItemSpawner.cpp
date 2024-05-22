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
	
	SpawnItem();
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrTime += DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("Tick..."));

	if (CurrTime >= SpawnInterval)
	{
		SpawnItem();

		CurrTime -= SpawnInterval;
	}
}

void AItemSpawner::SpawnItem()
{
	if (ItemToSpawn)
	{
		// Get the current location and rotation of the spawner
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		// Spawn the actor
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemToSpawn, SpawnLocation, SpawnRotation);

		//UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s, SpawnRotation: %s"), *SpawnLocation.ToString(), *SpawnRotation.ToString());

		if (SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Spawn"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemToSpawn is not set!"));
	}
}

