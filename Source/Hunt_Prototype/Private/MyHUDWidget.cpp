// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MyCharacterStatComponent.h"
#include "MyPlayerState.h"

void UMyHUDWidget::BindCharacterStat(UMyCharacterStatComponent* CharacterStat)
{
	HUNT_CHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UMyHUDWidget::UpdateCharacterStat);
}

void UMyHUDWidget::BindPlayerState(AMyPlayerState* PlayerState)
{
	HUNT_CHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UMyHUDWidget::UpdatePlayerState);
}

void UMyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	HUNT_CHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	HUNT_CHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	HUNT_CHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	HUNT_CHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	HUNT_CHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	HUNT_CHECK(nullptr != HighScore                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      );
}

void UMyHUDWidget::UpdateCharacterStat()
{
	HUNT_CHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void UMyHUDWidget::UpdatePlayerState()
{
	HUNT_CHECK(CurrentPlayerState.IsValid());

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));
}