// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterWidget.h"
#include "MyCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyCharacterWidget::BindCharacterStat(UMyCharacterStatComponent* NewCharacterStat)
{
	HUNT_CHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyCharacterWidget::UpdateHPWidget);
}

void UMyCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	HUNT_CHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UMyCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid()) {
		if (nullptr != HPProgressBar) {
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}