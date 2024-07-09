// Fill out your copyright notice in the Description page of Project Settings.


#include "BKChunkWorld.h"

#include "BKChunkBase.h"
#include "BKNaiveChunk.h"

#include "../Network/SGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABKChunkWorld::ABKChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABKChunkWorld::BeginPlay()
{
	Super::BeginPlay();

	switch (GenerationType)
	{
	case EGenerationType::GT_3D:
		Generate3DWorld();
		break;
	case EGenerationType::GT_2D:
		Generate2DWorld();
		break;
	default:
		throw std::invalid_argument("Invalid Generation Type");
	}
	UE_LOG(LogTemp, Warning, TEXT("%d Chunks Created"), ChunkCount);
}

void ABKChunkWorld::Tick(float DeltaTime)
{
	USGameInstance* instance = USGameInstance::GetMyInstance(this);
	if (!instance->BlockQueue.CheckEmpty()) {
		int BytesSent = 0;
		BlockInfo block;
		block = instance->BlockQueue.DeQ();
		ABKChunkBase* chunk = Cast<ABKChunkBase>(Chunks[block.chunk_index]);
		chunk->ProcessBlock(block);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Recv Add Block / Chunk_index: %d, x: %d, y: %d z: %d"), block.chunk_index, block.index.X, block.index.Y, block.index.Z));
	}
}

void ABKChunkWorld::Generate3DWorld()
{
	for (int x = -DrawDistance; x <= DrawDistance; ++x)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			for (int z = -DrawDistance; z <= DrawDistance; ++z)
			{
				auto transform = FTransform(
					FRotator::ZeroRotator,
					FVector(x * Size * 100, y * Size * 100, z * Size * 100),
					FVector::OneVector
				);

				const auto chunk = GetWorld()->SpawnActorDeferred<ABKChunkBase>(
					ChunkType,
					transform,
					this
				);

				chunk->GenerationType = EGenerationType::GT_3D;
				chunk->Frequency = Frequency;
				chunk->Material = Material;
				chunk->Size = Size;
				chunk->SetOwningChunkWorld(this); // 청크베이스의 부모로 월드를 연결

				UGameplayStatics::FinishSpawningActor(chunk, transform);

				Chunks.Add(chunk);

				// 맵 인덱스 출력을 위한 코드
			/*	int index = chunk->GetMyChunkIndex();
				ABKNaiveChunk* chunkNaive = Cast<ABKNaiveChunk>(chunk);
				chunkNaive->GetMapList();*/

				// 디버깅용
				FTransform actorTransform = chunk->GetTransform();
				FVector actorLocation = actorTransform.GetLocation();
				UE_LOG(LogTemp, Warning, TEXT("Add Chunk Location: (%f, %f, %f)"), actorLocation.X, actorLocation.Y, actorLocation.Z);


				ChunkCount++;
			}
		}
	}
}

void ABKChunkWorld::Generate2DWorld()
{
	for (int x = -DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			auto transform = FTransform(
				FRotator::ZeroRotator,
				FVector(x * Size * 100, y * Size * 100, 0),
				FVector::OneVector
			);

			const auto chunk = GetWorld()->SpawnActorDeferred<ABKChunkBase>(
				ChunkType,
				transform,
				this
			);

			chunk->GenerationType = EGenerationType::GT_2D;
			chunk->Frequency = Frequency;
			chunk->Material = Material;
			chunk->Size = Size;
			chunk->SetOwningChunkWorld(this);

			UGameplayStatics::FinishSpawningActor(chunk, transform);

			//Chunks.Add(chunk);

			// 디버깅용
			//FTransform actorTransform = chunk->GetTransform();
			//FVector actorLocation = actorTransform.GetLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Add Chunk Location: (%f, %f, %f)"), actorLocation.X, actorLocation.Y, actorLocation.Z);

			ChunkCount++;
		}
	}
}

ABKChunkWorld* ABKChunkWorld::FindOwningChunkWorld(ABKChunkBase* Chunk)
{
	// Chunk를 관리하는 Chunk World를 반환
	return Chunk ? Chunk->OwningChunkWorld : nullptr;
}

//int32 ABKChunkWorld::GetChunkIndex(AActor* Chunk, FIntVector Position, BKEBlock Block)
//{
//	// 배열을 순회하면서 Chunk를 찾는다
//	for (int32 Index = 0; Index < Chunks.Num(); ++Index)
//	{
//		if (Chunks[Index] == Chunk)
//		{
//			// Chunk를 참조하기 위한 Index,
//			// Modify Voxle에 필요한 인자 Position과 Block을 이용해서 서버 구현하면 될 듯!!
//			//
//
//
//
//
//
//			return Index; // Chunk를 찾으면 해당 인덱스를 반환
//		}
//	}
//	return INDEX_NONE; // Chunk가 배열에 없으면 INDEX_NONE 반환
//}

AActor* ABKChunkWorld::GetAdjacentActor(const AActor* ChunkActor, const FVector Normal)
{
	FVector AdjActorLocation = ChunkActor->GetActorLocation() + Normal * Size * 100;

	UE_LOG(LogTemp, Warning, TEXT("To Find: (%f, %f, %f)"), AdjActorLocation.X, AdjActorLocation.Y, AdjActorLocation.Z);

	for (int index = 0; index < Chunks.Num(); ++index)
	{
		FTransform actorTransform = Chunks[index]->GetTransform();
		FVector actorLocation = actorTransform.GetLocation();

		UE_LOG(LogTemp, Warning, TEXT("Finding...: (%f, %f, %f)"), actorLocation.X, actorLocation.Y, actorLocation.Z);

		if (FIntVector(actorLocation) == FIntVector(AdjActorLocation)) return Chunks[index];
	}
	return nullptr;
}