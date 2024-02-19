// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "blockers.h" //이게 맞는지 모르겠음
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKERS_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn,
		Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;
	
};
