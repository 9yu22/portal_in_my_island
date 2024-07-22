// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel/ChunkMeshData.h"
#include "Voxel/BKEnum.h"
#include "GameFramework/Actor.h"
#include "../Network/ProcessQueue.h"
#include "../Network/Protocol.h"
#include "BKChunkBase.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;
class ABKChunkWorld;

UCLASS(ABSTRACT)
class BLOCKERS_API ABKChunkBase : public AActor
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	ABKChunkBase();

	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int Size = 150;

	TObjectPtr<UMaterialInterface> Material;
	float Frequency;
	EGenerationType GenerationType;

	UFUNCTION(BlueprintCallable, Category = "Chunk")
	bool ModifyVoxel(const FIntVector Position, const BKEBlock Block);

	UFUNCTION(BlueprintCallable, Category = "Chunk")
	bool SendModifiedVoxel(const FVector World_Position, const FVector World_Normal, const FIntVector Position, const BKEBlock Block);

	// Reference to the owning ChunkWorld
	UPROPERTY()
	ABKChunkWorld* OwningChunkWorld;

	// Function to set the owning ChunkWorld
	void SetOwningChunkWorld(ABKChunkWorld* NewOwner);

	void ProcessBlock(const BlockInfo& Block);
	void AddBlock(const SC_ADD_BLOCK_PACKET& add_block);
	void RemoveBlock(const SC_REMOVE_BLOCK_PACKET& remove_block);
	void CreateBlockDestroyEffect(const FVector& world_index, const FVector& normal);
	int8 GetMyChunkIndex() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Setup() PURE_VIRTUAL(ABKChunkBase::Setup);
	virtual void Generate2DHeightMap(const FVector Position) PURE_VIRTUAL(ABKChunkBase::Generate2DHeightMap);
	virtual void Generate3DHeightMap(const FVector Position) PURE_VIRTUAL(ABKChunkBase::Generate3DHeightMap);
	virtual void GenerateMesh() PURE_VIRTUAL(ABKChunkBase::GenerateMesh);

	virtual bool ModifyVoxelData(const FIntVector Position, const BKEBlock Block) PURE_VIRTUAL(ABKChunkBase::ModifyVoxelData, return false;);

	TObjectPtr<UProceduralMeshComponent> Mesh;
	FastNoiseLite* Noise;
	FChunkMeshData MeshData;
	int VertexCount = 0;

public:
	void ApplyMesh() const;
	void ClearMesh();
	void GenerateHeightMap();
};
