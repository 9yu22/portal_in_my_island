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
				Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
			}
			for (int z = Height; z < Size; z++)
			{
				Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Air;
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
				auto distanceFromCenter = sqrt((x + Position.X) * (x + Position.X) + (y + Position.Y) * (y + Position.Y));
				auto distanceFromPoint1 = sqrt((x + Position.X - 80) * (x + Position.X - 80) + (y + Position.Y - 80) * (y + Position.Y - 80));	// point: (80,80)
				auto distanceFromPoint2 = sqrt((x + Position.X + 80) * (x + Position.X + 80) + (y + Position.Y - 80) * (y + Position.Y - 80));	// point: (-80,80)
				auto distanceFromPoint3 = sqrt((x + Position.X - 80) * (x + Position.X - 80) + (y + Position.Y + 80) * (y + Position.Y + 80));	// point: (80,-80)
				auto distanceFromPoint4 = sqrt((x + Position.X + 80) * (x + Position.X + 80) + (y + Position.Y + 80) * (y + Position.Y + 80));	// point: (-80,-80)

				// z = 0
				// point: (0,0) / radius: 40
				if (z + Position.Z == 0 && distanceFromCenter < 40) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (80,80) / radius: 15
				else if (z + Position.Z == 0 && distanceFromPoint1 < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-80,80) / radius: 15
				else if (z + Position.Z == 0 && distanceFromPoint2 < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (80,-80) / radius: 15
				else if (z + Position.Z == 0 && distanceFromPoint3 < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-80,-80) / radius: 15
				else if (z + Position.Z == 0 && distanceFromPoint4 < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// z = 1
				// point: (0,0) / radius: 30
				else if (z + Position.Z == 1 && distanceFromCenter < 30) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (80,80) / radius: 12
				else if (z + Position.Z == 1 && distanceFromPoint1 < 12) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-80,80) / radius: 12
				else if (z + Position.Z == 1 && distanceFromPoint2 < 12) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (80,-80) / radius: 12
				else if (z + Position.Z == 1 && distanceFromPoint3 < 12) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-80,-80) / radius: 12
				else if (z + Position.Z == 1 && distanceFromPoint4 < 12) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// z = 10
				// point: (0,0) / radius: 20~30
				else if (z + Position.Z == 10 && distanceFromCenter > 20 && distanceFromCenter < 30) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// °è´Ü
				// Forward
				else if (x + Position.X == z + Position.Z + 10 && y + Position.Y >= -1 && y + Position.Y <= 1 && z + Position.Z > 0 && z + Position.Z < 10) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				else if (x + Position.X == z + Position.Z + 11 && y + Position.Y == 0 && z + Position.Z == 9) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// Back
				else if (x + Position.X == -(z + Position.Z + 10) && y + Position.Y >= -1 && y + Position.Y <= 1 && z + Position.Z > 0 && z + Position.Z < 10) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Back;
				}
				else if (x + Position.X == -(z + Position.Z + 11) && y + Position.Y == 0 && z + Position.Z == 9) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// Right
				else if (x + Position.X >= -1 && x + Position.X <= 1 && y + Position.Y == z + Position.Z + 10 && z + Position.Z > 0 && z + Position.Z < 10) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Right;
				}
				else if (x + Position.X == 0 && y + Position.Y == z + Position.Z + 11 && z + Position.Z == 9) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// Left
				else if (x + Position.X >= -1 && x + Position.X <= 1 && y + Position.Y == -(z + Position.Z + 10) && z + Position.Z > 0 && z + Position.Z < 10) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Left;
				}
				else if (x + Position.X == 0 && y + Position.Y == -(z + Position.Z + 11) && z + Position.Z == 9) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				else {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Air;
				}
				//const auto NoiseValue = Noise->GetNoise(x + Position.X, y + Position.Y, z + Position.Z);
				//if (NoiseValue >= 0)
				//{
				//	Blocks[GetBlockIndex(x, y, z)] = BKEBlock::Air;
				//}
				//else
				//{
				//	Blocks[GetBlockIndex(x, y, z)] = BKEBlock::Stone;
				//}
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
				if (Blocks[GetBlockIndex(x, y, z)].block == BKEBlock::Stair)
				{
					const auto Position = FVector(x, y, z);

					for (auto Direction : { BKEDirection::Forward, BKEDirection::Right, BKEDirection::Back, BKEDirection::Left, BKEDirection::Up, BKEDirection::Down })
					{
						CreateFace(Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)], Direction, Position * 100);

					}
				}
				else if (Blocks[GetBlockIndex(x, y, z)].block != BKEBlock::Air)
				{
					const auto Position = FVector(x, y, z);

					for (auto Direction : { BKEDirection::Forward, BKEDirection::Right, BKEDirection::Back, BKEDirection::Left, BKEDirection::Up, BKEDirection::Down })
					{
						if (Check(GetPositionInDirection(Direction, Position)))
						{
							CreateFace(Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)], Direction, Position * 100);
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
	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)].block == BKEBlock::Air;
}

void ABKNaiveChunk::CreateFace(const Block Block, const BKEDirection Direction, const FVector Position)
{
	const auto Normal = GetNormal(Direction);
	const auto Color = FColor(0, 0, 0, GetTextureIndex(Block.block, Normal));

	if (Block.block == BKEBlock::Stair && Block.backDir == BKEDirection::Back)
	{
		FVector StairFowardVector = GetNormal(Block.backDir) * -1;
		FVector StairLeftVector = (StairFowardVector == FVector::ForwardVector) ? FVector::LeftVector :
			(StairFowardVector == FVector::RightVector) ? FVector::ForwardVector :
			(StairFowardVector == FVector::BackwardVector) ? FVector::RightVector :
			(StairFowardVector == FVector::LeftVector) ? FVector::BackwardVector : FVector(0);

		if (StairFowardVector == Normal)	// Forward
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), Normal * FVector(-0.5) + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector == Normal)	// Left
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 0.5, 0.5), FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector * -1 == Normal)	// Right
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 1, 0.5), FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (FVector::UpVector == Normal)	// Up
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 1, 1), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 1, 1), Normal * FVector(-0.5) + StairFowardVector * 0.5));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else
		{
			MeshData.Vertices.Append(GetFaceVertices(Direction, Position));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
	}
	else if (Block.block == BKEBlock::Stair && Block.backDir == BKEDirection::Forward)
	{
		FVector StairFowardVector = GetNormal(Block.backDir) * -1;
		FVector StairLeftVector = (StairFowardVector == FVector::ForwardVector) ? FVector::LeftVector :
			(StairFowardVector == FVector::RightVector) ? FVector::ForwardVector :
			(StairFowardVector == FVector::BackwardVector) ? FVector::RightVector :
			(StairFowardVector == FVector::LeftVector) ? FVector::BackwardVector : FVector(0);

		if (StairFowardVector == Normal)	// Forward
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), Normal * FVector(-0.5) + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector == Normal)	// Left
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 0.5, 0.5), StairFowardVector * -0.5 + Normal * 0.5 + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector * -1 == Normal)	// Right
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 1, 0.5), StairFowardVector * -0.5 + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (FVector::UpVector == Normal)	// Up
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 1, 1), StairFowardVector * -0.5));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 1, 1), Normal * FVector(-0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else
		{
			MeshData.Vertices.Append(GetFaceVertices(Direction, Position));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
	}
	else if (Block.block == BKEBlock::Stair && Block.backDir == BKEDirection::Right)
	{
		FVector StairFowardVector = GetNormal(Block.backDir) * -1;
		FVector StairLeftVector = (StairFowardVector == FVector::ForwardVector) ? FVector::LeftVector :
			(StairFowardVector == FVector::RightVector) ? FVector::ForwardVector :
			(StairFowardVector == FVector::BackwardVector) ? FVector::RightVector :
			(StairFowardVector == FVector::LeftVector) ? FVector::BackwardVector : FVector(0);

		if (StairFowardVector == Normal)	// Forward
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), Normal * FVector(-0.5) + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector == Normal)	// Left
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 0.5, 0.5), StairFowardVector * -0.5 + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector * -1 == Normal)	// Right
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 0.5, 0.5), StairFowardVector * -0.5 + FVector(0.5, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (FVector::UpVector == Normal)	// Up
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 0.5, 1), StairFowardVector * -0.5));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 0.5, 1), Normal * FVector(-0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else
		{
			MeshData.Vertices.Append(GetFaceVertices(Direction, Position));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
	}
	else if (Block.block == BKEBlock::Stair && Block.backDir == BKEDirection::Left)
	{
		FVector StairFowardVector = GetNormal(Block.backDir) * -1;
		FVector StairLeftVector = (StairFowardVector == FVector::ForwardVector) ? FVector::LeftVector :
			(StairFowardVector == FVector::RightVector) ? FVector::ForwardVector :
			(StairFowardVector == FVector::BackwardVector) ? FVector::RightVector :
			(StairFowardVector == FVector::LeftVector) ? FVector::BackwardVector : FVector(0);

		if (StairFowardVector == Normal)	// Forward
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), Normal * FVector(-0.5) + FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector == Normal)	// Left
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 0.5, 0.5), FVector(0.5, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (StairLeftVector * -1 == Normal)	// Right
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 1, 0.5), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(0.5, 0.5, 0.5), FVector(0, 0, 0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else if (FVector::UpVector == Normal)	// Up
		{
			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 0.5, 1), FVector(0, 0, 0)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;

			MeshData.Vertices.Append(GetStairFaceVertices(Direction, Position, FVector(1, 0.5, 1), StairFowardVector * 0.5 + Normal * FVector(-0.5)));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
		else
		{
			MeshData.Vertices.Append(GetFaceVertices(Direction, Position));
			MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
			MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
			MeshData.Colors.Append({ Color, Color, Color, Color });
			MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

			VertexCount += 4;
		}
	}
	else
	{
		MeshData.Vertices.Append(GetFaceVertices(Direction, Position));
		MeshData.Triangles.Append({ VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount });
		MeshData.Normals.Append({ Normal, Normal, Normal, Normal });
		MeshData.Colors.Append({ Color, Color, Color, Color });
		MeshData.UV0.Append({ FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1) });

		VertexCount += 4;
	}
}

TArray<FVector> ABKNaiveChunk::GetFaceVertices(BKEDirection Direction, const FVector Position) const
{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; i++)
	{
		Vertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] + Position);
	}

	return Vertices;
}

TArray<FVector> ABKNaiveChunk::GetStairFaceVertices(BKEDirection Direction, const FVector Position, const FVector Ratio, const FVector relationalPosition) const
{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; i++)
	{
		Vertices.Add(FVector(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]].X * Ratio.X + relationalPosition.X * 100,
			BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]].Y * Ratio.Y + relationalPosition.Y * 100,
			BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]].Z * Ratio.Z + relationalPosition.Z * 100)
			+ Position);
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

	Blocks[Index].block = Block;
}

int ABKNaiveChunk::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

int ABKNaiveChunk::GetTextureIndex(BKEBlock Block, FVector Normal) const
{
	// Block Texture Array //
	// 0: Amethyst
	// 1: Stone
	// 

	switch (Block)
	{
	case BKEBlock::Stone:
		return 1;
		break;
	/*case BKEBlock::Grass:
		if (Normal == FVector::UpVector) return 0;
		if (Normal == FVector::DownVector) return 2;
		return 1;*/
	case BKEBlock::Amethyst:
		return 0;
		break;
	default:
		return 255;
		break;
	}
}