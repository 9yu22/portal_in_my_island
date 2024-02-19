// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BKChunkBase.h"
#include "Voxel/BKEnum.h"
#include "BKNaiveChunk.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;

/**
 *
 */
UCLASS()
class ABKNaiveChunk final : public ABKChunkBase
{
	GENERATED_BODY()

	protected:
	virtual void Setup() override;
	virtual void Generate2DHeightMap(FVector Position) override;
	virtual void Generate3DHeightMap(FVector Position) override;
	virtual void GenerateMesh() override;
	virtual void ModifyVoxelData(const FIntVector Position, BKEBlock Block) override;
	virtual void GenerateSplitBlockMesh() override;
	virtual void RemoveSplitBlocks() override;

private:
	TArray<BKEBlock> Blocks;
	TArray<FVector> splitBlocks;	// 쪼개지는 중인 블록들의 좌표를 저장하는 배열

	int splitBlockNum = 4;			// 4 x 4 x 4로 쪼갠다는 의미
	const FVector BlockVertexData[8] = {
		FVector(100,100,100),
		FVector(100,0,100),
		FVector(100,0,0),
		FVector(100,100,0),
		FVector(0,0,100),
		FVector(0,100,100),
		FVector(0,100,0),
		FVector(0,0,0)
	};
	const int BlockTriangleData[24] = {
		0,1,2,3, // Forward
		5,0,3,6, // Right
		4,5,6,7, // Back
		1,4,7,2, // Left
		5,4,1,0, // Up
		3,2,7,6  // Down
	};

	bool Check(FVector Position) const;
	void CreateFace(BKEDirection Direction, FVector Position, bool isSplitBlock);
	TArray<FVector> GetFaceVertices(BKEDirection Direction, FVector Position, bool isSplitBlock) const;
	FVector GetPositionInDirection(BKEDirection Direction, FVector Position) const;
	FVector GetNormal(BKEDirection Direction) const;
	int GetBlockIndex(int X, int Y, int Z) const;
};
