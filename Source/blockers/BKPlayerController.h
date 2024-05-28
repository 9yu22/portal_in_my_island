// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Network/SGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "blockersCharacter.h"
#include "BKPlayerController.generated.h"


/**
 *
 */
UCLASS()
class BLOCKERS_API ABKPlayerController : public APlayerController
{
	GENERATED_BODY()

	//     읽기 전용           쓰기 전용            읽기 쓰기
	// BlueprintReadOnly / BlueprintWriteOnly / BlueprintReadWrite
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftClickAction;

protected:
	// 게임이 시작하는 BeginPlay (컨트롤러가 첫 번째 업데이트를 하기 전에 호출)
	virtual void BeginPlay() override;
	// 입력을 준비하는 함수
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	// 움직이라는 명령을 받음
	UFUNCTION()
	virtual void InputMove(const FInputActionValue& value);

	UFUNCTION()
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void StopJumping(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void OnLeftClick(const FInputActionValue& Value);

	UPROPERTY()
	UInputMappingContext* DefaultMappingContext;

	uint8 Anim_type = 0;

	//// 컨트롤하는 캐릭터를 반환
	AblockersCharacter* GetMyCharacter() const;

	//// 입력 키를 전송하는 함수
	void SendAnimation(int8 anim_type);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;
};
