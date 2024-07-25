// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordItem.h"
#include "../blockersCharacter.h"
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

		// LineTraceByChannel로 레이를 쏴서 충돌을 감지
		bHit = GetWorld()->LineTraceSingleByChannel(HitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility);

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
		}
		else
		{
			// 디버깅용: line Tracing의 빨간 라인 그리기
			DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
}