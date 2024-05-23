// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS(Blueprintable)
class BLOCKERS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FVector playerForward;

	UPROPERTY(EditAnywhere)
	FVector bulletDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHit = false;

	UPROPERTY(EditAnywhere)
	float moveSpeed = 800.0f;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* explosionFX;

	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Bullet")
	void FunctionWithDelay(float delayTime);

	void DestroyBullet();

	//UFUNCTION()
	//void OnBeginOverLap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromsweep, const FHitResult& SweepResult);
};
