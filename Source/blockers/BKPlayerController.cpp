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

		bool isFalling = GetCharacter()->GetCharacterMovement()->IsFalling();

		if (bShift && !isFalling)
		{
			if (prevLocation.Z == -100)
			{
				prevLocation = GetCharacter()->GetActorLocation();
			}

			if (GetCharacter()->GetActorLocation().Z < prevLocation.Z)
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
			prevLocation = {-100, -100, -100};
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
	//SendAnimation(static_cast<int8>(Anim::JUMP));
}

void ABKPlayerController::StopJumping(const FInputActionValue& Value)
{
	GetCharacter()->bPressedJump = false;
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

void ABKPlayerController::SendAnimation(int8 anim_type)
{
	if (Anim_type != anim_type) {

		USGameInstance* instance = USGameInstance::GetMyInstance(this);
		//instance->GetMyInstance(this);
		if (instance) {
			int BytesSent = 0;
			ANIM_PACKET p;
			p.size = sizeof(ANIM_PACKET);
			p.type = ANIM;
			p.id = GetMyCharacter()->id;
			p.anim_type = anim_type;
			instance->Socket->Send((uint8*)&p, sizeof(p), BytesSent);
		}
		else
			UE_LOG(LogTemp, Log, TEXT("Fail GetInstance"));
		Anim_type = anim_type;
	}

}

void ABKPlayerController::OnLeftClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Left Click!"));

	// 블록 아이템을 들고 있을 때만 블록 설치 가능
	/*ACharacter* MyCharacter = GetCharacter();
	if (MyCharacter)
	{
		AblockersCharacter* MyBlockersCharacter = Cast<AblockersCharacter>(MyCharacter);
		if (MyBlockersCharacter)
		{
			ABlockItem* blockItem = Cast<ABlockItem>(MyBlockersCharacter->SelectedItem);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}*/
	
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
					/*int32 ChunkIndex = OwningWorld->GetChunkIndex(HitChunk, LocalBlockPosition, BKEBlock::Air);
					UE_LOG(LogTemp, Log, TEXT("chunk index: %d"), ChunkIndex);*/

					// Modify Voxel
					//HitChunk->ModifyVoxel(LocalBlockPosition, BKEBlock::Air);
					bool removingSuccess = HitChunk->SendModifiedVoxel(HitResult.Location, HitResult.Normal, LocalBlockPosition, BKEBlock::Air);

					if (removingSuccess)
					{
						// BP_CollapsibleBlock 스폰
						FIntVector SpawnLocationInt = UBKVoxelFunctionLibrary::GetBlockWorldPostion(BlockWorldPosition, HitResult.Normal, 64);
						FVector SpawnLocationV = FVector(static_cast<float>(SpawnLocationInt.X), static_cast<float>(SpawnLocationInt.Y), static_cast<float>(SpawnLocationInt.Z));
						FRotator SpawnRotation = FRotator::ZeroRotator;		// 기본 회전값 사용

						UClass* CollapsibleBlockClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blockers/Blueprints/BP_CollapsibleBlock.BP_CollapsibleBlock_C"));
						if (CollapsibleBlockClass)
						{
							FActorSpawnParameters SpawnParams;
							AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(CollapsibleBlockClass, SpawnLocationV, SpawnRotation);

							if (SpawnedActor)
							{
								// CollapsibleBlockComponent 추가
								UClass* CollapsibleBlockComponentClass = UCollapsibleBlockComponent::StaticClass();
								UActorComponent* CollapsibleComponent = SpawnedActor->AddComponentByClass(CollapsibleBlockComponentClass, false, FTransform::Identity, false);

								if (CollapsibleComponent)
								{
									UE_LOG(LogTemp, Log, TEXT("CollapsibleBlockComponent Added"));

									UCollapsibleBlockComponent* CollapsibleBlockComponent = SpawnedActor->FindComponentByClass<UCollapsibleBlockComponent>();

									if (CollapsibleBlockComponent)
									{
										CollapsibleBlockComponent->DestroyActorWithDelay(3.0f);

										UClass* BombClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blockers/Blueprints/BP_Bomb.BP_Bomb_C"));
										if (BombClass)
										{
											AActor* SpawnedBomb = GetWorld()->SpawnActor<AActor>(BombClass, HitResult.ImpactPoint, SpawnRotation);
										}
									}
									else
									{
										UE_LOG(LogTemp, Warning, TEXT("UCollapsibleBlockComponent is NOT found"));
									}
								}
								else
								{
									UE_LOG(LogTemp, Warning, TEXT("CollapsibleBlockComponent Failed"));
								}
							}
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("BP_CollapsibleBlock Class Can't Be Loaded!"));
						}
					}
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
