// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel/BKEnum.h"
#include "GameFramework/Actor.h"
#include "BKChunkWorld.generated.h"

class ABKChunkBase;

UCLASS()
class ABKChunkWorld final : public AActor
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditInstanceOnly, Category = "World")
	TSubclassOf<ABKChunkBase> ChunkType;

	UPROPERTY(EditInstanceOnly, Category = "World")
	int DrawDistance = 5;

	UPROPERTY(EditInstanceOnly, Category = "Chunk")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditInstanceOnly, Category = "Chunk")
	int Size = 32;

	UPROPERTY(EditInstanceOnly, Category = "Height Map")
	EGenerationType GenerationType;

	UPROPERTY(EditInstanceOnly, Category = "Height Map")
	float Frequency = 0.03f;

	// Sets default values for this actor's properties
	ABKChunkWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int ChunkCount;

	void Generate3DWorld();
	void Generate2DWorld();
};
