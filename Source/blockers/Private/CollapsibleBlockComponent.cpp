// Fill out your copyright notice in the Description page of Project Settings.


#include "CollapsibleBlockComponent.h"

// Sets default values for this component's properties
UCollapsibleBlockComponent::UCollapsibleBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCollapsibleBlockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCollapsibleBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCollapsibleBlockComponent::DestroyActorWithDelay(float Delay)
{
    // Get the owner actor of this component
    AActor* OwnerActor = GetOwner();
    FTimerHandle TimerHandle;

    // Check if the owner actor is valid
    if (OwnerActor)
    {
        // Create a timer delegate to call DestroyActor function
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindLambda([OwnerActor]()
            {
                OwnerActor->Destroy();
            });

        // Set the timer to call DestroyActor function after the specified delay
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay, false);
    }
}