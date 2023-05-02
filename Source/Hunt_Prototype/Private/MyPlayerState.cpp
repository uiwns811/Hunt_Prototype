// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MyGameInstance.h"

AMyPlayerState::AMyPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 AMyPlayerState::GetGameScore() const 
{
	return GameScore;
}

int32 AMyPlayerState::GetCharacterLevel() const 
{
	return CharacterLevel;
}

void AMyPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	SetCharacterLevel(5);
	GameScore = 0;
	Exp = 0;
}

float AMyPlayerState::GetExpRatio() const 
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER) return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	HUNT_LOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AMyPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1) return false;

	bool DidLevelUp = false;
	Exp += IncomeExp;
	if (Exp >= CurrentStatData->NextExp) {
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	return DidLevelUp;
}

void AMyPlayerState::AddGameScore()
{
	GameScore++;
	OnPlayerStateChanged.Broadcast();
}

void AMyPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	HUNT_CHECK(nullptr != MyGameInstance);

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewCharacterLevel);
	HUNT_CHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
