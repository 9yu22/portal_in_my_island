// Fill out your copyright notice in the Description page of Project Settings.


#include "BKNaiveChunk.h"

#include "Voxel/FastNoiseLite.h"

void ABKNaiveChunk::Setup()
{
	// Initialize Blocks
	Blocks.SetNum(Size * Size * Size);
}

void ABKNaiveChunk::Generate2DHeightMap(const FVector Position)
{
	for (int x = 0; x < Size; x++)
	{
		for (int y = 0; y < Size; y++)
		{
			const float Xpos = x + Position.X;
			const float ypos = y + Position.Y;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, ypos) + 1) * Size / 2), 0, Size);
			for (int z = 0; z < Height; z++)
			{
				Blocks[GetBlockIndex(x, y, z)] = BKEBlock::Stone;
			}
			for (int z = Height; z < Size; z++)
			{
				Blocks[GetBlockIndex(x, y, z)] = BKEBlock::Air;
			}

		}
	}
}

void ABKNaiveChunk::Generate3DHeightMap(const FVector Position)
{
	for (int x = 0; x < Size; ++x)
	{
		for (int y = 0; y < Size; ++y)
		{
			for (int z = 0; z < Size; ++z)
			{
				const auto NoiseValue = Noise->GetNoise(x + Position.X, y + Position.Y, z + Position.Z);
				if (NoiseValue >= 0)
				{
					Blocks[GetBlockIndex(x, y, z)] = BKEBlock::Air;
				}
				else
				{
					Blocks[GetBlockIndex(x, y, z)] = BKEBlock::Stone;
				}
			}
		}
	}
}

void ABKNaiveChunk::GenerateMesh()
{
	for (int x = 0; x < Size; x++)
	{
		for (int y = 0; y < Size; y++)
		{
			for (int z = 0; z < Size; z++)
			{
				if (Blocks[GetBlockIndex(x, y, z)] != BKEBlock::Air)
				{
					const auto Position = FVector(x, y, z);

					for (auto Direction : { BKEDirection::Forward, BKEDirection::Right, BKEDirection::Back, BKEDirection::Left, BKEDirection::Up, BKEDirection::Down })
					{
						if (Check(GetPositionInDirection(Direction, Position)))
						{
							CreateFace(Direction, Position * 100, false);
						}
					}
				}
			}
		}
	}
}

bool ABKNaiveChunk::Check(const FVector Position) const
{
	if (Position.X >= Size || Position.Y >= Size || Position.X < 0 || Position.Y < 0 || Position.Z < 0) return true;
	if (Position.Z >= Size) return true;
	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)] == BKEBlock::Air;
}

void ABKNaiveChunk::CreateFace(const BKEDirection Direction, const FVector Position, bool isSplitBlock)
{
	const auto Color = FColor::MakeRandomColor();
	const auto Normal = GetNormal(Direction);

	MeshData.Vertices.Append(GetFaceVertices(Direction, Position, isSplitBlock));
	MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
	MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
	MeshData.Colors.Append({ Color, Color, Color, Color });
	MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

	VertexCount += 4;
}

TArray<FVector> ABKNaiveChunk::GetFaceVertices(BKEDirection Direction, const FVector Position, bool isSplitBlock) const
{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; i++)
	{
		if (isSplitBlock)
			Vertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] * 1 / (float)splitBlockNum * 0.8f + Position);
		else
			Vertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] + Position);
	}

	return Vertices;
}

FVector ABKNaiveChunk::GetPositionInDirection(const BKEDirection Direction, const FVector Position) const
{
	switch (Direction)
	{
	case BKEDirection::Forward: return Position + FVector::ForwardVector;
	case BKEDirection::Back: return Position + FVector::BackwardVector;
	case BKEDirection::Left: return Position + FVector::LeftVector;
	case BKEDirection::Right: return Position + FVector::RightVector;
	case BKEDirection::Up: return Position + FVector::UpVector;
	case BKEDirection::Down: return Position + FVector::DownVector;
	default: throw std::invalid_argument("Invalid direction");
	}
}

FVector ABKNaiveChunk::GetNormal(const BKEDirection Direction) const
{
	switch (Direction)
	{
	case BKEDirection::Forward: return FVector::ForwardVector;
	case BKEDirection::Right: return FVector::RightVector;
	case BKEDirection::Back: return FVector::BackwardVector;
	case BKEDirection::Left: return FVector::LeftVector;
	case BKEDirection::Up: return FVector::UpVector;
	case BKEDirection::Down: return FVector::DownVector;
	default: throw std::invalid_argument("Invalid direction");
	}
}

void ABKNaiveChunk::ModifyVoxelData(const FIntVector Position, const BKEBlock Block)
{
	const int Index = GetBlockIndex(Position.X, Position.Y, Position.Z);

	Blocks[Index] = Block;

	// 전달 받은 Block이 Air라는 것 => 파괴하고자 하는 블록
	// 그러면 해당 블록은 Air로 만들고 그 위치를 저장해서 8등분한다.
	if (Block == BKEBlock::Air)
	{
		splitBlocks.Add(Position);
	}
}

void ABKNaiveChunk::GenerateSplitBlockMesh()
{
	// 일단 작은 블록 8개만 그려보자
	if (!splitBlocks.IsEmpty())
	{
		float splitNum = 1 / (float)splitBlockNum;

		for (auto splitBlockPosition : splitBlocks)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Small Block Position: (%f, %f, %f)"), splitBlockPosition.X, splitBlockPosition.Y, splitBlockPosition.Z);

			for (int x = 0; x < splitBlockNum; ++x)
			{
				for (int y = 0; y < splitBlockNum; ++y)
				{
					for (int z = 0; z < splitBlockNum; ++z)
					{
						for (auto Direction : { BKEDirection::Forward, BKEDirection::Right, BKEDirection::Back, BKEDirection::Left, BKEDirection::Up, BKEDirection::Down })
						{
							CreateFace(Direction, FVector(splitBlockPosition.X + splitNum * x, splitBlockPosition.Y + splitNum * y, splitBlockPosition.Z + splitNum * z) * 100, true);
						}
					}
				}
			}
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("There are NOT mini blocks"));
}

int ABKNaiveChunk::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

void ABKNaiveChunk::RemoveSplitBlocks()
{
	if (!splitBlocks.IsEmpty())
	{
		splitBlocks.RemoveAt(0);
	}
}