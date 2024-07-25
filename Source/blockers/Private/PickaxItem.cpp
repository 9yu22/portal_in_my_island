// Fill out your copyright notice in the Description page of Project Settings.


#include "PickaxItem.h"

void APickaxItem::UseItem()
{
	Durability -= Usage;
}
