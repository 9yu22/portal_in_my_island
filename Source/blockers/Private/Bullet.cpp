// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PracticeDestroyBlock.h"
#include "../blockersCharacter.h"
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

	bulletDirection = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Quaternion().GetForwardVector();

	boxComp->OnComponentBeginOverlap;

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	FVector startLocation = GetActorLocation() + bulletDirection * moveSpeed * DeltaTime;


	SetActorLocation(startLocation);

	FVector endLocation = GetActorLocation() + bulletDirection * 100;

	// LineTraceByChannel로 레이를 쏴서 충돌을 감지
	bHit = GetWorld()->LineTraceSingleByChannel(HitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility);

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

	AblockersCharacter* enemy2 = Cast<AblockersCharacter>(OtherActor);
	
	if (enemy2 != nullptr)
	{
		//OtherActor->Destroy();
		enemy2->health -= 10;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation(), GetActorRotation());
	}

	Destroy();

}

void ABullet::FunctionWithDelay(float delayTime)
{
	// 새로운 타이머 핸들을 생성합니다.
	FTimerHandle NewTimerHandle;

	// 2초 후에 호출될 함수를 예약합니다.
	GetWorld()->GetTimerManager().SetTimer(NewTimerHandle, this, &ABullet::DestroyBullet, delayTime, false);
}

void ABullet::DestroyBullet()
{
	Destroy();
}

