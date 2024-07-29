// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolItem.h"

void APistolItem::UseItem()
{
	Durability -= Usage;
}

float APistolItem::GetBlockDamage()
{
	return 0.0f;
}

float APistolItem::GetPlayerDamage()
{
	return 0.0f;
}
