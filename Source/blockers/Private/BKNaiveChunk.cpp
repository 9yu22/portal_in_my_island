// Fill out your copyright notice in the Description page of Project Settings.


#include "BKNaiveChunk.h"
#include "../Network/SGameInstance.h"

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
				auto distanceFromPoint1 = sqrt((x + Position.X - 110) * (x + Position.X - 110) + (y + Position.Y - 110) * (y + Position.Y - 110));	// point: (110,110)
				auto distanceFromPoint2 = sqrt((x + Position.X + 110) * (x + Position.X + 110) + (y + Position.Y - 110) * (y + Position.Y - 110));	// point: (-110,110)
				auto distanceFromPoint3 = sqrt((x + Position.X - 110) * (x + Position.X - 110) + (y + Position.Y + 110) * (y + Position.Y + 110));	// point: (110,-110)
				auto distanceFromPoint4 = sqrt((x + Position.X + 110) * (x + Position.X + 110) + (y + Position.Y + 110) * (y + Position.Y + 110));	// point: (-110,-110)

				auto distanceFromPoint5 = sqrt((x + Position.X) * (x + Position.X) + (y + Position.Y - 70) * (y + Position.Y - 70));	// point: (0, 70)
				auto distanceFromPoint6 = sqrt((x + Position.X) * (x + Position.X) + (y + Position.Y + 70) * (y + Position.Y + 70));	// point: (0, -70)
				auto distanceFromPoint7 = sqrt((x + Position.X - 70) * (x + Position.X - 70) + (y + Position.Y) * (y + Position.Y));	// point: (70, 0)
				auto distanceFromPoint8 = sqrt((x + Position.X + 70) * (x + Position.X + 70) + (y + Position.Y) * (y + Position.Y));	// point: (-70, 0)

				auto distanceFromPoint9 = sqrt((x + Position.X - 20) * (x + Position.X - 20) + (y + Position.Y - 20) * (y + Position.Y - 20));	// point: (20,20)
				auto distanceFromPoint10 = sqrt((x + Position.X + 20) * (x + Position.X + 20) + (y + Position.Y - 20) * (y + Position.Y - 20));	// point: (-20,20)
				auto distanceFromPoint11 = sqrt((x + Position.X - 20) * (x + Position.X - 20) + (y + Position.Y + 20) * (y + Position.Y + 20));	// point: (20,-20)
				auto distanceFromPoint12 = sqrt((x + Position.X + 20) * (x + Position.X + 20) + (y + Position.Y + 20) * (y + Position.Y + 20));	// point: (-20,-20)


				// 기본 맵은 부술 수 없음.
				Blocks[GetBlockIndex(x, y, z)].isCollapsible = false;

				Blocks[GetBlockIndex(x, y, z)].health = -2;		// 부술 수 없는 블록의 health

				BKEBlock groundBlock;
				USGameInstance* GameInstance = USGameInstance::GetMyInstance(this);
				if (GameInstance->selectedMap == 0)
					groundBlock = BKEBlock::Grass;
				else if (GameInstance->selectedMap == 1)
					groundBlock = BKEBlock::Snow;
				else
					groundBlock = BKEBlock::HellGround;

				// z = 0
				// point: (0,0) / radius: 65
				if (z + Position.Z == 0 && distanceFromCenter < 65) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (110,110) / radius: 20
				else if (z + Position.Z == 0 && distanceFromPoint1 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-110,110) / radius: 20
				else if (z + Position.Z == 0 && distanceFromPoint2 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (110,-110) / radius: 20
				else if (z + Position.Z == 0 && distanceFromPoint3 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-110,-110) / radius: 20
				else if (z + Position.Z == 0 && distanceFromPoint4 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// z = 1
				// point: (0,0) / radius: 30
				else if (z + Position.Z == 1 && distanceFromCenter < 30) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (110,110) / radius: 17
				else if (z + Position.Z == 1 && distanceFromPoint1 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-110,110) / radius: 17
				else if (z + Position.Z == 1 && distanceFromPoint2 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (110,-110) / radius: 17
				else if (z + Position.Z == 1 && distanceFromPoint3 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// point: (-110,-110) / radius: 17
				else if (z + Position.Z == 1 && distanceFromPoint4 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// z = 15
				// point: (0,0) / radius: 30~50
				else if (z + Position.Z == 15 && distanceFromCenter > 30 && distanceFromCenter < 50) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
				}
				// 계단
				// Forward
				else if (x + Position.X == z + Position.Z + 15 && y + Position.Y >= -1 && y + Position.Y <= 1 && z + Position.Z > 0 && z + Position.Z < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				else if (x + Position.X == z + Position.Z + 16 && y + Position.Y == 0 && z + Position.Z == 14) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// Back
				else if (x + Position.X == -(z + Position.Z + 15) && y + Position.Y >= -1 && y + Position.Y <= 1 && z + Position.Z > 0 && z + Position.Z < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Back;
				}
				else if (x + Position.X == -(z + Position.Z + 16) && y + Position.Y == 0 && z + Position.Z == 14) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// Right
				else if (x + Position.X >= -1 && x + Position.X <= 1 && y + Position.Y == z + Position.Z + 15 && z + Position.Z > 0 && z + Position.Z < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Right;
				}
				else if (x + Position.X == 0 && y + Position.Y == z + Position.Z + 16 && z + Position.Z == 14) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// Left
				else if (x + Position.X >= -1 && x + Position.X <= 1 && y + Position.Y == -(z + Position.Z + 15) && z + Position.Z > 0 && z + Position.Z < 15) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stair;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Left;
				}
				else if (x + Position.X == 0 && y + Position.Y == -(z + Position.Z + 16) && z + Position.Z == 14) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Stone;
					Blocks[GetBlockIndex(x, y, z)].backDir = BKEDirection::Forward;
				}
				// *** 중앙섬 꾸미기 *** //
				// 바깥쪽 언덕 4개
				// point: (0,70) / radius: 35
				else if (z + Position.Z == 0 && distanceFromPoint5 < 35) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,-70) / radius: 35
				else if (z + Position.Z == 0 && distanceFromPoint6 < 35) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (70,0) / radius: 35
				else if (z + Position.Z == 0 && distanceFromPoint7 < 35) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-70,0) / radius: 35
				else if (z + Position.Z == 0 && distanceFromPoint8 < 35) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,70) / radius: 25
				else if (z + Position.Z == 1 && distanceFromPoint5 < 25) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,-70) / radius: 25
				else if (z + Position.Z == 1 && distanceFromPoint6 < 25) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (70,0) / radius: 25
				else if (z + Position.Z == 1 && distanceFromPoint7 < 25) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-70,0) / radius: 25
				else if (z + Position.Z == 1 && distanceFromPoint8 < 25) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,70) / radius: 21
				else if (z + Position.Z == 2 && distanceFromPoint5 < 21) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,-70) / radius: 21
				else if (z + Position.Z == 2 && distanceFromPoint6 < 21) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (70,0) / radius: 21
				else if (z + Position.Z == 2 && distanceFromPoint7 < 21) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-70,0) / radius: 21
				else if (z + Position.Z == 2 && distanceFromPoint8 < 21) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,70) / radius: 17
				else if (z + Position.Z == 3 && distanceFromPoint5 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,-70) / radius: 17
				else if (z + Position.Z == 3 && distanceFromPoint6 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (70,0) / radius: 17
				else if (z + Position.Z == 3 && distanceFromPoint7 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-70,0) / radius: 17
				else if (z + Position.Z == 3 && distanceFromPoint8 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// 중앙쪽 언덕 4개
				// point: (0,70) / radius: 20
				else if (z + Position.Z == 1 && distanceFromPoint9 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (0,-70) / radius: 20
				else if (z + Position.Z == 1 && distanceFromPoint10 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (70,0) / radius: 20
				else if (z + Position.Z == 1 && distanceFromPoint11 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-70,0) / radius: 20
				else if (z + Position.Z == 1 && distanceFromPoint12 < 20) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,20) / radius: 17
				else if (z + Position.Z == 2 && distanceFromPoint9 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,20) / radius: 17
				else if (z + Position.Z == 2 && distanceFromPoint10 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,-20) / radius: 17
				else if (z + Position.Z == 2 && distanceFromPoint11 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,-20) / radius: 17
				else if (z + Position.Z == 2 && distanceFromPoint12 < 17) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,20) / radius: 14
				else if (z + Position.Z == 3 && distanceFromPoint9 < 14) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,20) / radius: 14
				else if (z + Position.Z == 3 && distanceFromPoint10 < 14) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,-20) / radius: 14
				else if (z + Position.Z == 3 && distanceFromPoint11 < 14) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,-20) / radius: 14
				else if (z + Position.Z == 3 && distanceFromPoint12 < 14) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,20) / radius: 11
				else if (z + Position.Z == 4 && distanceFromPoint9 < 11) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,20) / radius: 11
				else if (z + Position.Z == 4 && distanceFromPoint10 < 11) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,-20) / radius: 11
				else if (z + Position.Z == 4 && distanceFromPoint11 < 11) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,-20) / radius: 11
				else if (z + Position.Z == 4 && distanceFromPoint12 < 11) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,20) / radius: 7
				else if (z + Position.Z == 5 && distanceFromPoint9 < 7) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,20) / radius: 7
				else if (z + Position.Z == 5 && distanceFromPoint10 < 7) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (20,-20) / radius: 7
				else if (z + Position.Z == 5 && distanceFromPoint11 < 7) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// point: (-20,-20) / radius: 7
				else if (z + Position.Z == 5 && distanceFromPoint12 < 7) {
					Blocks[GetBlockIndex(x, y, z)].block = groundBlock;
				}
				// 나무
				/*else if (ableToCreateObject(x, y, z, Position, {-20, -20, 6}, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -20, -20, 6 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 20, 20, 6 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 20, 20, 6 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -20, 20, 6 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -20, 20, 6 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 20, -20, 6 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 20, -20, 6 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 0, -35, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 0, -35, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 0, 35, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 0, 35, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -35, 0, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -35, 0, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 35, 0, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 35, 0, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}*/
				/*else if (ableToCreateObject(x, y, z, Position, { 40, 40, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 40, 40, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 35, 44, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 35, 44, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 44, 35, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 44, 35, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 30, 48, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 30, 48, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { 48, 30, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { 48, 30, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}*/
				/*else if (ableToCreateObject(x, y, z, Position, { -40, -40, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -40, -40, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -35, -44, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -35, -44, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -44, -35, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -44, -35, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -30, -48, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -30, -48, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -48, -30, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -48, -30, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -40, 40, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -40, 40, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -35, 44, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -35, 44, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -44, 35, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -44, 35, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -30, 48, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -30, 48, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}
				else if (ableToCreateObject(x, y, z, Position, { -48, 30, 1 }, "spruce")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Spruce;
				}
				else if (ableToCreateObject(x, y, z, Position, { -48, 30, 1 }, "leaves")) {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Leaves;
				}*/
				else {
					Blocks[GetBlockIndex(x, y, z)].block = BKEBlock::Air;
					Blocks[GetBlockIndex(x, y, z)].health = -1;		// 부술 수 있는 블록의 health
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

	TArray<FVector> Spruce = {
		{ -20, -20, 6 },
		{ 20, 20, 6 },
		{ -20, 20, 6 },
		{ 20, -20, 6 },
		{ 0, -35, 1 },
		{ 0, 35, 1 },
		{ -35, 0, 1 },
		{ 35, 0, 1 }
	};

	CreateObjects(0, 0, 0, Position);
}

void ABKNaiveChunk::CreateTreeAtPosition(double x, double y, double z)
{
	// 나무를 구성하는 좌표
	TArray<FVector> spruceCoordinates = {
		{0, 0, 0}, {0, 0, 1}, {0, 0, 2}
	};

	// 잎 블록을 구성하는 좌표
	TArray<FVector> leavesCoordinates = {
		{0, 0, 4},
		{0, -1, 3}, {0, 1, 3}, {-1, 0, 3}, {1, 0, 3},
		{0, -1, 2}, {0, 1, 2}, {-1, 0, 2}, {1, 0, 2},
		{1, 1, 2}, {-1, -1, 2}, {-1, 1, 2}, {1, -1, 2}
	};

	// 나무 블록 생성
	for (const FVector& coord : spruceCoordinates)
	{
		int32 targetX = x + coord.X;
		int32 targetY = y + coord.Y;
		int32 targetZ = z + coord.Z;
		Blocks[GetBlockIndex(targetX, targetY, targetZ)].block = BKEBlock::Spruce;
	}

	// 잎 블록 생성
	for (const FVector& coord : leavesCoordinates)
	{
		int32 targetX = x + coord.X;
		int32 targetY = y + coord.Y;
		int32 targetZ = z + coord.Z;
		if (targetX >= 0 && targetY >= 0 && targetZ >= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Leaves Pos - x:%f, y:%f, z:%f"), x + coord.X, y + coord.Y, z + coord.Z);
			Blocks[GetBlockIndex(targetX, targetY, targetZ)].block = BKEBlock::Leaves;
		}
	}
}

void ABKNaiveChunk::CreatePillarAtPosition(double x, double y, double z)
{
	// 기본 기둥을 구성하는 좌표
	TArray<FVector> baseCoordinates = {

		{0, 0, 0}, {1, 0, 0}, {2, 0, 0},
		{0, 1, 0}, {1, 1, 0}, {2, 1, 0},
		{0, 2, 0}, {1, 2, 0}, {2, 2, 0},

		{0, 0, 1}, {1, 0, 1}, {2, 0, 1},
		{0, 1, 1}, {1, 1, 1}, {2, 1, 1},
		{0, 2, 1}, {1, 2, 1}, {2, 2, 1},

		{0, 0, 2}, {1, 0, 2}, {2, 0, 2},
		{0, 1, 2}, {1, 1, 2}, {2, 1, 2},
		{0, 2, 2}, {1, 2, 2}, {2, 2, 2}
	};

	// 기본 기둥 외에 상하좌우 추가 블럭을 구성하는 좌표
	TArray<FVector> decoCoordinates = {
		{1, 1, 3}, {0, 1, 3}, {1, 0, 3}, {2, 1, 3}, {1, 2, 3}, {1, 1, 4},

		{0, 3, 0}, {1, 3, 0}, {2, 3, 0}, {1, 3, 1},

		{3, 0, 0}, {3, 1, 0}, {3, 2, 0}, {3, 1, 1},

		{-1, 0, 0}, {-1, 1, 0}, {-1, 2, 0}, {-1, 1, 1},

		{0, -1, 0}, {1, -1, 0}, {2, -1, 0}, {1, -1, 1}
	};

	// 기본 블록 생성
	for (const FVector& coord : baseCoordinates)
	{
		int32 targetX = x + coord.X;
		int32 targetY = y + coord.Y;
		int32 targetZ = z + coord.Z;

		if (targetX >= 0 && targetY >= 0 && targetZ >= 0)
			Blocks[GetBlockIndex(targetX, targetY, targetZ)].block = BKEBlock::IcePillar;
	}

	// 추가 블록 생성
	for (const FVector& coord : decoCoordinates)
	{
		int32 targetX = x + coord.X;
		int32 targetY = y + coord.Y;
		int32 targetZ = z + coord.Z;

		if (targetX >= 0 && targetY >= 0 && targetZ >= 0)
			Blocks[GetBlockIndex(targetX, targetY, targetZ)].block = BKEBlock::IcePillar;
	}
}


void ABKNaiveChunk::CreateObjects(int32 x, int32 y, int32 z, FVector Position)
{
	// 나무를 생성할 위치 목록
	TArray<FVector> positionsToCheck = {
		{-20, -20, 6}, {20, 20, 6}, {-20, 20, 6}, {20, -20, 6},
		//{0, -35, 1}, {0, 35, 1}, {-35, 0, 1}, {35, 0, 1},
		{40, 40, 1}, {35, 44, 1}, {44, 35, 1}, {30, 48, 1}, {48, 30, 1},
		{-40, 40, 1}, {-35, 44, 1}, {-44, 35, 1}, {-30, 48, 1}, {-48, 30, 1},
		{40, -40, 1}, {35, -44, 1}, {44, -35, 1}, {30, -48, 1}, {48, -30, 1},
		{-40, -40, 1}, {-35, -44, 1}, {-44, -35, 1}, {-30, -48, 1}, {-48, -30, 1}
	};

	// 각 위치에 나무를 생성
	for (const FVector& offset : positionsToCheck)
	{
		UE_LOG(LogTemp, Warning, TEXT("Making Map... x:%f, y:%f, z:%f"), x + offset.X - Position.X, y + offset.Y - Position.Y, z + offset.Z - Position.Z);
		if (x + offset.X - Position.X >= 0 && x + offset.X - Position.X < (float)Size
			&& y + offset.Y - Position.Y >= 0 && y + offset.Y - Position.Y < (float)Size
			&& z + offset.Z - Position.Z >= 0 && z + offset.Z - Position.Z < (float)Size)
		{
			
			CreateTreeAtPosition(x + offset.X - Position.X, y + offset.Y - Position.Y, z + offset.Z - Position.Z);
		}
	}

	// 얼음기둥을 생성할 위치 목록
	TArray<FVector> positionsToCheckPillar = {
		{60, 10, 4}, {60, -10, 4}, {-65, 10, 4}, {-65, -10, 4}
	};

	// 각 위치에 얼음기둥을 생성
	for (const FVector& offset : positionsToCheckPillar)
	{
		if (x + offset.X - Position.X >= 0 && x + offset.X - Position.X < (float)Size
			&& y + offset.Y - Position.Y >= 0 && y + offset.Y - Position.Y < (float)Size
			&& z + offset.Z - Position.Z >= 0 && z + offset.Z - Position.Z < (float)Size)

			CreatePillarAtPosition(x + offset.X - Position.X, y + offset.Y - Position.Y, z + offset.Z - Position.Z);
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
				if (Blocks[GetBlockIndex(x, y, z)].block == BKEBlock::Stair || Blocks[GetBlockIndex(x, y, z)].block == BKEBlock::Leaves)
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
	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)].block == BKEBlock::Air || Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)].block == BKEBlock::Stair || Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)].block == BKEBlock::Leaves;
}

void ABKNaiveChunk::CreateFace(const FBlockk Block, const BKEDirection Direction, const FVector Position)
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

bool ABKNaiveChunk::ModifyVoxelData(const FIntVector Position, const BKEBlock Block)
{
	const int Index = GetBlockIndex(Position.X, Position.Y, Position.Z);

	// 삭제하려 했지만 삭제할 수 없는 블록이면 패스
	if (Block == BKEBlock::Air) {
		if (Blocks[Index].isCollapsible == false) { return false; }
		else {
			/*if (Blocks[Index].health > 0) {
				Blocks[Index].health -= 20;
			}*/
			Blocks[Index].block = Block;
			Blocks[Index].health = -1;

			return true;
			
			/*if (Blocks[Index].health <= 0) {
				Blocks[Index].block = Block;
				return true;
			}
			else if (Blocks[Index].health <= 25) {
				if (Blocks[Index].block == BKEBlock::StoneB2)
					Blocks[Index].block = BKEBlock::StoneB3;
				else if (Blocks[Index].block == BKEBlock::AmethystB2)
					Blocks[Index].block = BKEBlock::AmethystB3;
				else if (Blocks[Index].block == BKEBlock::RubyB2)
					Blocks[Index].block = BKEBlock::RubyB3;
				else if (Blocks[Index].block == BKEBlock::DiamondB2)
					Blocks[Index].block = BKEBlock::DiamondB3;
			}
			else if (Blocks[Index].health <= 50) {
				if (Blocks[Index].block == BKEBlock::StoneB1)
					Blocks[Index].block = BKEBlock::StoneB2;
				else if (Blocks[Index].block == BKEBlock::AmethystB1)
					Blocks[Index].block = BKEBlock::AmethystB2;
				else if (Blocks[Index].block == BKEBlock::RubyB1)
					Blocks[Index].block = BKEBlock::RubyB2;
				else if (Blocks[Index].block == BKEBlock::DiamondB1)
					Blocks[Index].block = BKEBlock::DiamondB2;
			}
			else if (Blocks[Index].health <= 75) {
				if (Blocks[Index].block == BKEBlock::Stone)
					Blocks[Index].block = BKEBlock::StoneB1;
				else if (Blocks[Index].block == BKEBlock::Amethyst)
					Blocks[Index].block = BKEBlock::AmethystB1;
				else if (Blocks[Index].block == BKEBlock::Ruby)
					Blocks[Index].block = BKEBlock::RubyB1;
				else if (Blocks[Index].block == BKEBlock::Diamond)
					Blocks[Index].block = BKEBlock::DiamondB1;
			}*/
		}
	}
	else {
		if (Blocks[Index].health == -1)
		{
			Blocks[Index].isCollapsible = true;
			Blocks[Index].health = 100;
		}
		Blocks[Index].block = Block;
		return true;
	}
	return false;
}

int ABKNaiveChunk::GetBlockIndex(const int X, const int Y, const int Z) const
{
	return Z * Size * Size + Y * Size + X;
}

BKEBlock ABKNaiveChunk::GetBreakingBlock(FIntVector Position)
{
	const int Index = GetBlockIndex(Position.X, Position.Y, Position.Z);

	if (Blocks[Index].health > 0) {
		Blocks[Index].health -= 20;
	}

	if (Blocks[Index].health <= 0) {
		if (Blocks[Index].block == BKEBlock::StoneB3)
			CollapsibleBlockName = "Stone";
		else if (Blocks[Index].block == BKEBlock::AmethystB3)
			CollapsibleBlockName = "Amethyst";
		else if (Blocks[Index].block == BKEBlock::RubyB3)
			CollapsibleBlockName = "Ruby";
		else if (Blocks[Index].block == BKEBlock::DiamondB3)
			CollapsibleBlockName = "Diamond";
		return BKEBlock::Air;
	}
	else if (Blocks[Index].health <= 25) {
		if (Blocks[Index].block == BKEBlock::StoneB2)
			return BKEBlock::StoneB3;
		else if (Blocks[Index].block == BKEBlock::AmethystB2)
			return BKEBlock::AmethystB3;
		else if (Blocks[Index].block == BKEBlock::RubyB2)
			return BKEBlock::RubyB3;
		else if (Blocks[Index].block == BKEBlock::DiamondB2)
			return BKEBlock::DiamondB3;
	}
	else if (Blocks[Index].health <= 50) {
		if (Blocks[Index].block == BKEBlock::StoneB1)
			return BKEBlock::StoneB2;
		else if (Blocks[Index].block == BKEBlock::AmethystB1)
			return BKEBlock::AmethystB2;
		else if (Blocks[Index].block == BKEBlock::RubyB1)
			return BKEBlock::RubyB2;
		else if (Blocks[Index].block == BKEBlock::DiamondB1)
			return BKEBlock::DiamondB2;
	}
	else if (Blocks[Index].health <= 75) {
		if (Blocks[Index].block == BKEBlock::Stone)
			return BKEBlock::StoneB1;
		else if (Blocks[Index].block == BKEBlock::Amethyst)
			return BKEBlock::AmethystB1;
		else if (Blocks[Index].block == BKEBlock::Ruby)
			return BKEBlock::RubyB1;
		else if (Blocks[Index].block == BKEBlock::Diamond)
			return BKEBlock::DiamondB1;
	}
	return Blocks[Index].block;
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
		return 0;
		break;
	case BKEBlock::Stair:
		return 0;
		break;
	case BKEBlock::Amethyst:
		return 1;
		break;
	case BKEBlock::Ruby:
		return 2;
		break;
	case BKEBlock::Diamond:
		return 3;
		break;
	case BKEBlock::Mycelium:
		if (Normal == FVector::UpVector) return 5;
		if (Normal == FVector::DownVector) return 4;
		return 6;
		break;
	case BKEBlock::Grass:
		if (Normal == FVector::UpVector) return 7;
		if (Normal == FVector::DownVector) return 4;
		return 8;
		break;
	case BKEBlock::Leaves:
		return 9;
		break;
	case BKEBlock::Spruce:
		if (Normal == FVector::UpVector || Normal == FVector::DownVector) return 11;
		return 10;
		break;
	case BKEBlock::Snow:
		if (Normal == FVector::UpVector) return 12;
		if (Normal == FVector::DownVector) return 4;
		return 13;
		break;
	case BKEBlock::HellGround:
		return 14;
		break;
	case BKEBlock::StoneB1:
		return 15;
		break;
	case BKEBlock::StoneB2:
		return 16;
		break;
	case BKEBlock::StoneB3:
		return 17;
		break;
	case BKEBlock::AmethystB1:
		return 18;
		break;
	case BKEBlock::AmethystB2:
		return 19;
		break;
	case BKEBlock::AmethystB3:
		return 20;
		break;
	case BKEBlock::RubyB1:
		return 21;
		break;
	case BKEBlock::RubyB2:
		return 22;
		break;
	case BKEBlock::RubyB3:
		return 23;
		break;
	case BKEBlock::DiamondB1:
		return 24;
		break;
	case BKEBlock::DiamondB2:
		return 25;
		break;
	case BKEBlock::DiamondB3:
		return 26;
		break;
	case BKEBlock::IcePillar:
		return 27;
		break;
	default:
		return 255;
		break;
	}
}

bool ABKNaiveChunk::ableToCreateObject(const int x, const int y, const int z, const FVector Position, const FVector startPosition, const std::string objName)
{
	if (objName == "spruce")
	{
		return (x + Position.X == startPosition.X && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z) ||
			(x + Position.X == startPosition.X && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 1) ||
			(x + Position.X == startPosition.X && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 2);
	}
	else if (objName == "leaves")
	{
		return (x + Position.X == startPosition.X && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 4) ||
			(x + Position.X == startPosition.X && y + Position.Y == startPosition.Y - 1 && z + Position.Z == startPosition.Z + 3) ||
			(x + Position.X == startPosition.X && y + Position.Y == startPosition.Y + 1 && z + Position.Z == startPosition.Z + 3) ||
			(x + Position.X == startPosition.X - 1 && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 3) ||
			(x + Position.X == startPosition.X + 1 && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 3) ||
			(x + Position.X == startPosition.X && y + Position.Y == startPosition.Y - 1 && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X && y + Position.Y == startPosition.Y + 1 && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X - 1 && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X + 1 && y + Position.Y == startPosition.Y && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X + 1 && y + Position.Y == startPosition.Y + 1 && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X - 1 && y + Position.Y == startPosition.Y - 1 && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X - 1 && y + Position.Y == startPosition.Y + 1 && z + Position.Z == startPosition.Z + 2) ||
			(x + Position.X == startPosition.X + 1 && y + Position.Y == startPosition.Y - 1 && z + Position.Z == startPosition.Z + 2);
	}
	else
	{
		
	}
	return false; // 기본 반환 값
}

void ABKNaiveChunk::GetMapList()
{
	FString FilePath = FPaths::ProjectDir() / TEXT("Blockers_map.txt");

	for (int i = 0; i < Blocks.Num(); ++i) {
		if (Blocks[i].block != BKEBlock::Air) {
			FIntVector temp = FindBlockindex(i);
			int blocktype = static_cast<int>(Blocks[i].block);
			int chunk_index = static_cast<int>(GetMyChunkIndex());
			//UE_LOG(LogTemp, Warning, TEXT("Chunk: %d, X: %d, Y: %d, Z: %d, BlockType: %d"), chunk_index, temp.X, temp.Y, temp.Z, blocktype);
			FString Line = FString::Printf(TEXT("%d %d %d %d %d\n"), chunk_index, temp.X, temp.Y, temp.Z, blocktype);  // 저장할 값을 공백으로 구분하여 문자열로 변환
			FFileHelper::SaveStringToFile(Line, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
		}
	}
}

FIntVector ABKNaiveChunk::FindBlockindex(int k)
{
	int32 z = k / (64 * 64);
	k = k % (64 * 64);
	int32 y = k / 64;
	int32 x = k % 64;

	return FIntVector(x, y, z);
}