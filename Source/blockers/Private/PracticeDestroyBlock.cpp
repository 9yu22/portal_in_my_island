// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeDestroyBlock.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APracticeDestroyBlock::APracticeDestroyBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	meshComp->SetupAttachment(boxComp);

}

// Called when the game starts or when spawned
void APracticeDestroyBlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APracticeDestroyBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
