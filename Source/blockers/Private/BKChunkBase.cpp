// Fill out your copyright notice in the Description page of Project Settings.

#include "BKChunkBase.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "../Network/SGameInstance.h"
#include "../Network/protocol.h"

#include "Voxel/BKEnum.h"
#include "ProceduralMeshComponent.h"

#include "Voxel/FastNoiseLite.h"


// Sets default values
ABKChunkBase::ABKChunkBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Noise = new FastNoiseLite();

	// Mesh Settings
	Mesh->SetCastShadow(true);

	// Set Mesh as root
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ABKChunkBase::BeginPlay()
{
	Super::BeginPlay();

	Noise->SetFrequency(Frequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);

	Setup();

	GenerateHeightMap();

	GenerateMesh();

	UE_LOG(LogTemp, Warning, TEXT("Vertex Cont : %d"), VertexCount);

	ApplyMesh();
}

void ABKChunkBase::GenerateHeightMap()
{
	switch (GenerationType)
	{
	case EGenerationType::GT_3D:
		Generate3DHeightMap(GetActorLocation() / 100);
		break;
	case EGenerationType::GT_2D:
		Generate2DHeightMap(GetActorLocation() / 100);
		break;
	default:
		throw std::invalid_argument("Invalid Generation Type");
	}
}

void ABKChunkBase::ApplyMesh() const
{
	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.Colors, TArray<FProcMeshTangent>(), true);
}

void ABKChunkBase::ClearMesh()
{
	VertexCount = 0;
	MeshData.Clear();
}

void ABKChunkBase::ModifyVoxel(const FIntVector Position, const BKEBlock Block)
{
	//if (Position.X >= Size || Position.Y >= Size || Position.Z >= Size || Position.X < 0 || Position.Y < 0 || Position.Z < 0) return Position;

	ModifyVoxelData(Position, Block);

	ClearMesh();

	GenerateMesh();

	ApplyMesh();
}

void ABKChunkBase::SendVoxel(const FIntVector Position, const BKEBlock Block)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Enter voxel")));
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		USGameInstance* SGameInstance = Cast<USGameInstance>(GameInstance);
		if (SGameInstance && SGameInstance->Socket)
		{
			// 소켓 사용 예제
			FSocket* Socket = SGameInstance->Socket;
			int32 BytesSent = 0;
			SC_ADD_BLOCK_PACKET block;
			block.size = sizeof(block);
			block.type = CS_ADD_BLOCK;
			block.ix = Position.X;
			block.iy = Position.Y;
			block.iz = Position.Z;
			block.blocktype = static_cast<uint8>(Block);
			Socket->Send(reinterpret_cast<uint8*>(&block), sizeof(block), BytesSent);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Send Block Index, x: %f, y: %f, z: %f") , Position.X, Position.Y, Position.Z));

			if (BytesSent > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Sent %d bytes to the server"), BytesSent);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to send data to the server"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SGameInstance is null or Socket is not initialized"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is null"));
	}
}
