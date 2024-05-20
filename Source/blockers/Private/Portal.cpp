// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Portalhealth = 100.f;
	
	// Create and initialize Sphere Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	// Set Sphere Radius and Collision Profile
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	GetSphereComponent()->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnBeginOverLap);
	if (PortalHpWidgetClass != nullptr)
	{
		PortalHpWidget = CreateWidget(GetWorld(), PortalHpWidgetClass);
		PortalHpWidget->AddToViewport();
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Test")) //이게 캐릭터여야함
	{
		Portalhealth -= 10.f;
		if (Portalhealth < 1) {
			UGameplayStatics::OpenLevel(this, "End");
		}
	}
}

