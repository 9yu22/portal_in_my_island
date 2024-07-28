// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BKChunkBase.h"
#include "Voxel/BKEnum.h"
#include "../Network/ProcessQueue.h"
#include "BKNaiveChunk.generated.h"

class FastNoiseLite;
class UProceduralMeshComponent;


// 이런식으로 못하니까 큐를 맴버 변수로 만들고 포인터 getter 만들어서 통신 스레드 쪽에서 포인터로 접근하도록 해보자
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
	virtual bool ModifyVoxelData(const FIntVector Position, BKEBlock Block) override;

	void CreateTreeAtPosition(double x, double y, double z);
	void CreateObjects(int32 x, int32 y, int32 z, FVector Position);

	struct Block {
		BKEBlock block;
		BKEDirection backDir;
		int8 health;
		bool isCollapsible;
	};

public:
	TArray<Block> Blocks;

	// 맵 정보를 알아내 서버에 파일로 저장하기 위해 만든 두 함수
	void GetMapList();
	FIntVector FindBlockindex(int index);

private:
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
	void CreateFace(const Block Block, BKEDirection Direction, FVector Position);
	TArray<FVector> GetFaceVertices(BKEDirection Direction, FVector Position) const;
	TArray<FVector> GetStairFaceVertices(BKEDirection Direction, const FVector Position, const FVector Ratio, const FVector relationalPosition) const;
	FVector GetPositionInDirection(BKEDirection Direction, FVector Position) const;
	FVector GetNormal(BKEDirection Direction) const;
	int GetBlockIndex(int X, int Y, int Z) const;

	UFUNCTION(BlueprintPure, Category="BlockInfo")
	BKEBlock GetBreakingBlock(FIntVector Position);	// 인자: 블록 포지션 (월드 포지션 X)

	int GetTextureIndex(BKEBlock Block, FVector Normal) const;

	bool ableToCreateObject(const int x, const int y, const int z, const FVector Position, const FVector startPosition, const std::string objName);
};

