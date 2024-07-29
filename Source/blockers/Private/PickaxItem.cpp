// Fill out your copyright notice in the Description page of Project Settings.


#include "PickaxItem.h"

void APickaxItem::UseItem()
{
	Durability -= Usage;
}

float APickaxItem::GetBlockDamage()
{
	if (GetPickaxType(this) == 3)	// diamond
		return 100.f;
	return 20.0f;
}

float APickaxItem::GetPlayerDamage()
{
	return 0.0f;
}

int8 APickaxItem::GetPickaxType(APickaxItem* MyActor) const
{
	const FString BlueprintPath = TEXT("/Game/Blockers/Blueprints/Item/BP_PickaxItem_Diamond.BP_PickaxItem_Diamond_C");

	if (MyActor == nullptr || BlueprintPath.IsEmpty())
	{
		return -1;
	}

	// 블루프린트 클래스를 로드
	UClass* BlueprintClass = StaticLoadClass(UObject::StaticClass(), nullptr, *BlueprintPath);
	if (BlueprintClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Loaded: %s"), *BlueprintPath);
		return -1;
	}

	// MyActor가 로드된 블루프린트 클래스의 인스턴스인지 확인
	if (MyActor->IsA(BlueprintClass))
		return 3;

	return -1;
}
