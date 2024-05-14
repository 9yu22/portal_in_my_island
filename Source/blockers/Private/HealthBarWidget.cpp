// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	
	//Super::NativeTick(MyGeometry, InDeltaTime);
	//

	//HealthBar->SetPercent(OwnerCharacter->GetHealth() / OwnerCharacter->GetMaxHealth());

	//FNumberFormattingOptions opts;
	//opts.SetMaximumFractionalDigits(0);
	//CurrentHealthLabel->SetText(FText::AsNumber(OwnerCharacter->GetHealth(), &opts));
	//MaxHealthLabel->SetText(FText::AsNumber(OwnerCharacter->GetMaxHealth(), &opts));
}
