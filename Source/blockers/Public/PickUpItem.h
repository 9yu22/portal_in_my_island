// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

UCLASS()
class BLOCKERS_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 회전 정도를 결정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	FRotator RotationRate;

	// 루트 컴포넌트가 될 SceneComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	USceneComponent* SceneComponent;

	// Mesh 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	UStaticMeshComponent* ItemMesh;

	//회전 스피드를 결정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	float Speed;
};
