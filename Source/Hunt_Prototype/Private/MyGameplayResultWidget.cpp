// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "MyGameState.h"

void UMyGameplayResultWidget::BindGameState(AMyGameState* GameState)
{
	HUNT_CHECK(nullptr != GameState);
	CurrentGameState = GameState;
}

void UMyGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HUNT_CHECK(CurrentGameState.IsValid());

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	HUNT_CHECK(nullptr != Result);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Mission Complete!") : TEXT("Mission Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	HUNT_CHECK(nullptr != TotalScore);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}