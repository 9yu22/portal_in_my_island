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

    if (Portalhealth == 0)
    {
        UGameplayStatics::OpenLevel(this, FName("End"));
    }

}

void APortal::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromsweep, const FHitResult& SweepResult)
{
 
    if (OtherActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("OtherActor is valid"));
        if (OtherActor->ActorHasTag("Test"))
        {

            UE_LOG(LogTemp, Warning, TEXT("OtherActor has 'Test' tag"));
            Portalhealth -= 10.f;
            
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OtherActor does not have 'Test' tag"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OtherActor is null"));
    }

}

