// Fill out your copyright notice in the Description page of Project Settings.


#include "BKVoxelFunctionLibrary.h"

FIntVector UBKVoxelFunctionLibrary::WorldToBlockPosition(const FVector& Position)
{
	return FIntVector(Position) / 100;
}

FIntVector UBKVoxelFunctionLibrary::WorldToLocalBlockPosition(const FVector& Position, const int Size)
{
	const auto ChunkPosition = WorldToChunkPosition(Position, Size);

	auto Result = WorldToBlockPosition(Position) - ChunkPosition * Size;

	// Negative Normalization
	if (ChunkPosition.X < 0) Result.X--;
	if (ChunkPosition.Y < 0) Result.Y--;
	if (ChunkPosition.Z < 0) Result.Z--;

	return Result;
}

FIntVector UBKVoxelFunctionLibrary::WorldToChunkPosition(const FVector& Position, const int Size)
{
	FIntVector Result;

	const int Factor = Size * 100;
	const auto IntPosition = FIntVector(Position);

	if (IntPosition.X < 0) Result.X = (int)(Position.X / Factor) - 1;
	else Result.X = (int)(Position.X / Factor);

	if (IntPosition.Y < 0) Result.Y = (int)(Position.Y / Factor) - 1;
	else Result.Y = (int)(Position.Y / Factor);

	if (IntPosition.Z < 0) Result.Z = (int)(Position.Z / Factor) - 1;
	else Result.Z = (int)(Position.Z / Factor);

	return Result;
}