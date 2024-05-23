// Fill out your copyright notice in the Description page of Project Settings.


#include "BKPlayerController.h"
#include "BKChunkBase.h"			// for HitResult
#include "BKChunkWorld.h"			// for server
#include "BKVoxelFunctionLibrary.h"	// for localBlockPosition

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "HealthBarWidget.h"

#include "UObject/Class.h"
#include "Kismet/KismetMathLibrary.h"	// for getFowardVector(), getRightVector()


void ABKPlayerController::BeginPlay()
{
	// 원본의 BeginPlay
	// 부모
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void ABKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABKPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABKPlayerController::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABKPlayerController::InputMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABKPlayerController::Look);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &ABKPlayerController::OnLeftClick);
	}

}

void ABKPlayerController::InputMove(const FInputActionValue& value)
{
	FVector2D inputAxis = value.Get<FVector2D>();

	if (APawn* controlledPawn = GetPawn())
	{
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
		FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());

		controlledPawn->AddMovementInput(ForwardVector, inputAxis.Y);
		controlledPawn->AddMovementInput(RightVector, inputAxis.X);
	}
}


void ABKPlayerController::Jump(const FInputActionValue& Value)
{
	GetCharacter()->bPressedJump = true;
}

void ABKPlayerController::StopJumping(const FInputActionValue& Value)
{
	GetCharacter()->bPressedJump = false;
}

void ABKPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	GetCharacter()->AddControllerYawInput(LookAxisVector.X);
	GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);
}

void ABKPlayerController::OnLeftClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Left Click!"));

	if (PlayerCameraManager)
	{
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();

		FVector startLocation = CameraLocation;

		FVector endLocation = CameraLocation + CameraRotation.Vector() * 1000;	// 1000은 거리

		// LineTraceByChannel로 레이를 쏴서 충돌을 감지
		bHit = GetWorld()->LineTraceSingleByChannel(HitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility);

		if (bHit)
		{
			// 디버깅용: line Tracing의 빨간 라인 그리기
			DrawDebugLine(GetWorld(), startLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);

			// 디버깅용: line에 부딪힌 위치에 초록색 구 그리기
			DrawDebugSphere(GetWorld(), HitResult.Location, 5.0f, 12, FColor::Green, false, 1.0f);

			UE_LOG(LogTemp, Log, TEXT("Hit!"));

			// Hit된 액터가 Chunk인가?
			ABKChunkBase* HitChunk = Cast<ABKChunkBase>(HitResult.GetActor());

			if (HitChunk)
			{
				// Hit Chunk를 관리하는 Chunk World를 불러온다. (World로 Hit Chunk를 보내 index를 찾기 위함)
				ABKChunkWorld* OwningWorld = ABKChunkWorld::FindOwningChunkWorld(HitChunk);
				if (OwningWorld)
				{
					UE_LOG(LogTemp, Log, TEXT("World Hit!"));

					// Block의 월드 위치
					FVector BlockWorldPosition = HitResult.Location - HitResult.Normal;
					// Block 배열 내 위치
					FIntVector LocalBlockPosition = UBKVoxelFunctionLibrary::WorldToLocalBlockPosition(BlockWorldPosition, HitResult.Normal, 64);

					// 디버깅용: Chunk Index 불러옴
					int32 ChunkIndex = OwningWorld->GetChunkIndex(HitChunk, LocalBlockPosition, BKEBlock::Air);
					UE_LOG(LogTemp, Log, TEXT("chunk index: %d"), ChunkIndex);
				}
			}
		}
		else
		{
			// 라인이 아무 것도 맞추지 않았을 때, 전체 길이의 빨간 라인을 그립니다.
			DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
}
