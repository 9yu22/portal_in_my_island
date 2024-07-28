// Fill out your copyright notice in the Description page of Project Settings.


#include "BKPlayerController.h"
#include "BKChunkBase.h"			// for HitResult
#include "BKChunkWorld.h"			// for server
#include "BKVoxelFunctionLibrary.h"	// for localBlockPosition
#include "CollapsibleBlockComponent.h"	// for collapsible Block
#include "PickUpItem.h"
#include "BlockItem.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"

#include "UObject/Class.h"
#include "Kismet/KismetMathLibrary.h"	// for getFowardVector(), getRightVector()


void ABKPlayerController::BeginPlay()
{
	// 원본의 BeginPlay
	// 부모
	Super::BeginPlay();

	BlockersCharacter = Cast<AblockersCharacter>(GetCharacter());

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

	}

}

void ABKPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APawn* controlledPawn = GetPawn())
	{
		FVector Velocity = controlledPawn->GetVelocity();
		bool bIsMovingNow = !Velocity.IsNearlyZero();
		if (!bIsMovingNow && Anim_type == static_cast<int8>(Anim::MOVE))// 움직이다 멈추면 패킷 전송
		{
			//SendAnimation(static_cast<int8>(Anim::IDLE));
		}
	}
}

void ABKPlayerController::InputMove(const FInputActionValue& value)
{
	FVector2D inputAxis = value.Get<FVector2D>();

	if (APawn* controlledPawn = GetPawn())
	{
		// 컨트롤러의 회전값을 가져옵니다
		FRotator Rotation = GetControlRotation();

		// pitch를 0으로 설정하여 수정합니다
		FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawRotation);
		FVector RightVector = UKismetMathLibrary::GetRightVector(YawRotation);

		controlledPawn->AddMovementInput(ForwardVector, inputAxis.Y);
		controlledPawn->AddMovementInput(RightVector, inputAxis.X);

		bool isFalling = BlockersCharacter->BIsInAir;

		// Check a point in front of the player to see if there is ground
		FVector CheckPoint = BlockersCharacter->GetActorLocation() + (BlockersCharacter->GetActorForwardVector() * -30.0f) + FVector(0.0f, 0.0f, -110.0f);

		FHitResult HitResultt;
		FCollisionQueryParams CollisionParams;

		bool bIsOnGround = GetWorld()->LineTraceSingleByChannel(HitResultt, BlockersCharacter->GetActorLocation(), CheckPoint, ECC_Visibility, CollisionParams);


		if (bShift && !isFalling)
		{
			if (prevLocation.Z == -100)
			{
				prevLocation = GetCharacter()->GetActorLocation();
			}

			if (!bIsOnGround)
			{
				GetCharacter()->SetActorLocation(prevLocation);
			}
			else
			{
				
			}
			prevLocation = GetCharacter()->GetActorLocation();
		}
		else if (bShift && isFalling)
		{
			if (BlockersCharacter->BIsJumping)
				prevLocation = {-100, -100, -100};
			else
			{
				if (prevLocation.Z == -100)
				{
					prevLocation = GetCharacter()->GetActorLocation();
				}

				if (!bIsOnGround)
				{
					GetCharacter()->SetActorLocation(prevLocation);
				}
				else
				{

				}
				prevLocation = GetCharacter()->GetActorLocation();
			}

		}
		else
		{
			prevLocation = GetCharacter()->GetActorLocation();
		}
	}
} 


void ABKPlayerController::Jump(const FInputActionValue& Value)
{
	GetCharacter()->bPressedJump = true;
	BlockersCharacter->BIsJumping = true;
	//SendAnimation(static_cast<int8>(Anim::JUMP));
}

void ABKPlayerController::StopJumping(const FInputActionValue& Value)
{
	GetCharacter()->bPressedJump = false;
	BlockersCharacter->BIsJumping = false;
	//SendAnimation(static_cast<int8>(Anim::IDLE));
}

void ABKPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	GetCharacter()->AddControllerYawInput(LookAxisVector.X);
	GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);
}

AblockersCharacter* ABKPlayerController::GetMyCharacter() const
{
	return Cast<AblockersCharacter>(GetPawn());
}
