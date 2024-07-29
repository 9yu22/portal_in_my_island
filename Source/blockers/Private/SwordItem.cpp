// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordItem.h"
#include "../blockersCharacter.h"
#include "BKNaiveChunk.h"
#include "../Network/SGameInstance.h"


void ASwordItem::UseItem()
{
	APlayerCameraManager* PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (PlayerCameraManager)
	{
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();

		FVector startLocation = CameraLocation;

		FVector endLocation = CameraLocation + CameraRotation.Vector() * 200;	// 거리: 200


		APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (PlayerPawn)
		{
			// FCollisionQueryParams를 사용하여 트레이스 파라미터 설정
			FCollisionQueryParams TraceParams(FName(TEXT("SwordTrace")), true, PlayerPawn);
			TraceParams.bTraceComplex = true;
			TraceParams.bReturnPhysicalMaterial = false;

			// LineTraceByChannel로 레이를 쏴서 충돌을 감지
			bHit = GetWorld()->LineTraceSingleByChannel(HitResult, startLocation, endLocation, ECollisionChannel::ECC_GameTraceChannel6, TraceParams);

			if (bHit)
			{
				// 디버깅용: line Tracing의 빨간 라인 그리기
				DrawDebugLine(GetWorld(), startLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);

				// 디버깅용: line에 부딪힌 위치에 초록색 구 그리기
				DrawDebugSphere(GetWorld(), HitResult.Location, 5.0f, 12, FColor::Green, false, 1.0f);

				AblockersCharacter* TargetCharacter = Cast<AblockersCharacter>(HitResult.GetActor());

				if (TargetCharacter)
				{
					// 내구도 감소
					Durability -= Usage;

					USGameInstance* GameInstance = USGameInstance::GetMyInstance(this);
					if (GameInstance)
					{
						for (AblockersCharacter* Player : GameInstance->Players)
						{
							if (Player == TargetCharacter)
							{
								//Player->health -= 10;

								if (GameInstance && GameInstance->Socket != nullptr) {
									CS_CHANGE_PLAYER_HP_PACKET p;
									p.size = sizeof(p);
									p.type = CS_CHANGE_PLAYER_HP;
									p.hit_id = Player->id;

									int BytesSent = 0;
									GameInstance->Socket->Send((uint8*)&p, sizeof(p), BytesSent);
								}

								break;
							}
						}
					}
				}

				ABKNaiveChunk* TargetChunk = Cast<ABKNaiveChunk>(HitResult.GetActor());

				if (TargetChunk)
				{
					// 내구도 감소
					Durability -= Usage;
				}
			}
			else
			{
				// 디버깅용: line Tracing의 빨간 라인 그리기
				DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 1.0f, 0, 1.0f);
			}
		}
	}
}

float ASwordItem::GetBlockDamage()
{
	if (IsCastToDiamond(this))
		return 100.f;
	return 20.0f;
}

float ASwordItem::GetPlayerDamage()
{
	return 0.0f;
}

bool ASwordItem::IsCastToDiamond(ASwordItem* MyActor) const
{
	const FString& BlueprintPath = "Blueprint'Blockers/Blueprints/Item/BP_PickaxItem_Diamond.BP_PickaxItem_Diamond_C'";

	if (MyActor == nullptr || BlueprintPath.IsEmpty())
	{
		return false;
	}

	// 블루프린트 클래스를 로드
	UBlueprint* LoadedBlueprint = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *BlueprintPath));
	if (LoadedBlueprint == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("블루프린트를 로드할 수 없습니다: %s"), *BlueprintPath);
		return false;
	}

	UClass* BlueprintClass = LoadedBlueprint->GeneratedClass;
	if (BlueprintClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("블루프린트 클래스가 유효하지 않습니다: %s"), *BlueprintPath);
		return false;
	}

	// MyActor가 로드된 블루프린트 클래스의 인스턴스인지 확인
	return MyActor->IsA(BlueprintClass);
}
