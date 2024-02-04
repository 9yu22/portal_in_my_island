// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PracticeDestroyBlock.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	boxComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	playerForward = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();

	boxComp->OnComponentBeginOverlap;

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	FVector newLocation = GetActorLocation() + playerForward * moveSpeed * DeltaTime;
	

	SetActorLocation(newLocation);

}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	APracticeDestroyBlock* enemy = Cast<APracticeDestroyBlock>(OtherActor);

	if (enemy != nullptr)
	{
		OtherActor->Destroy();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation(), GetActorRotation());
	}

	Destroy();

}

