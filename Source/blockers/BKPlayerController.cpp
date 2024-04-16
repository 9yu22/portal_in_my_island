// Fill out your copyright notice in the Description page of Project Settings.


#include "BKPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "UObject/Class.h"




void ABKPlayerController::BeginPlay()
{
	// 원본의 BeginPlay
	// 부모
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
	player2 = GetWorld()->SpawnActor<AblockersCharacter>(AblockersCharacter::StaticClass(), FVector(890.0f, 2000.0f, 950.0f/*여기에 초기 위치 넣어주면 됨*/), FRotator(0.0f, 0.0f, 0.0f));

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
	}

	
}

void ABKPlayerController::InputMove(const FInputActionValue& value)
{
	FVector2D inputAxis = value.Get<FVector2D>();

	if (ACharacter* controlledCharacter = GetCharacter())
	{
		//// 회전 속도 및 이동 속도 변수 설정
		//float RotationRate = 90.0f;
		//float MovementSpeed = 500.0f;

		//// 회전 속도 계산
		//FRotator Rotation = ControlledCharacter->GetControlRotation();
		//FRotator YawRotation(0, Rotation.Yaw, 0);
		//FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//// 이동과 회전에 속도 적용
		//ControlledCharacter->AddMovementInput(ForwardDirection, InputAxis.Y * MovementSpeed * GetWorld()->GetDeltaSeconds());
		//ControlledCharacter->AddMovementInput(RightDirection, InputAxis.X * MovementSpeed * GetWorld()->GetDeltaSeconds());

		//// 회전 속도 적용
		//AddYawInput(InputAxis.X * RotationRate * GetWorld()->GetDeltaSeconds());
		const FRotator Rotation = controlledCharacter->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		GetCharacter()->AddMovementInput(ForwardDirection, inputAxis.Y);
		GetCharacter()->AddMovementInput(RightDirection, inputAxis.X);
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