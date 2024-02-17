// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel/ChunkMeshData.h"
#include "Voxel/BKEnum.h"
#include "GameFramework/Actor.h"
#include "BKChunkBase.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS(ABSTRACT)
class BLOCKERS_API ABKChunkBase : public AActor
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	ABKChunkBase();

	UPROPERTY(EditDefaultsOnly, Category = "Chunk")
	int Size = 64;

	TObjectPtr<UMaterialInterface> Material;
	float Frequency;
	EGenerationType GenerationType;

	UFUNCTION(BlueprintCallable, Category = "Chunk")
	void ModifyVoxel(const FIntVector Position, const BKEBlock Block);

	UFUNCTION(BlueprintCallable, Category = "Chunk")
	void FunctionWithDelay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Setup() PURE_VIRTUAL(ABKChunkBase::Setup);
	virtual void Generate2DHeightMap(const FVector Position) PURE_VIRTUAL(ABKChunkBase::Generate2DHeightMap);
	virtual void Generate3DHeightMap(const FVector Position) PURE_VIRTUAL(ABKChunkBase::Generate3DHeightMap);
	virtual void GenerateMesh() PURE_VIRTUAL(ABKChunkBase::GenerateMesh);
	virtual void GenerateSplitBlockMesh() PURE_VIRTUAL(ABKChunkBase::GenerateSplitBlockMesh);	// 미니 블록 그리는 함수
	virtual void RemoveSplitBlocks() PURE_VIRTUAL(ABKChunkBase::RemoveSplitBlocks);				// 미니 블록 배열 비우는 함수

	virtual void ModifyVoxelData(const FIntVector Position, const BKEBlock Block) PURE_VIRTUAL(ABKChunkBase::RemoveVoxelData);

	TObjectPtr<UProceduralMeshComponent> Mesh;
	FastNoiseLite* Noise;
	FChunkMeshData MeshData;
	int VertexCount = 0;

public:
	void ApplyMesh() const;
	void ClearMesh();
	void GenerateHeightMap();
	void FinishSplitBlocksAnimation();
};
