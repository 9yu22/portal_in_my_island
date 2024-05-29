// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionItem.h"
#include "../blockersCharacter.h"

void UPotionItem::Use(AblockersCharacter* Character)
{
	if (Character)
	{
		if(Character->health<90.f)
		Character->health += HealthToHeal; //HealthToHeal = 10.f 
	}
}
