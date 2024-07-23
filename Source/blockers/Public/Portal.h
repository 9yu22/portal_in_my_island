// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class BLOCKERS_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetHealth() const { return Portalhealth; }
	void SetHealth(float val) { Portalhealth = val; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Portalhealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int id = -1;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PortalHpWidgetClass;
	UUserWidget* PortalHpWidget;

	UFUNCTION()
	void OnBeginOverLap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromsweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	USphereComponent* GetSphereComponent() const { return SphereComponent; }

};
