// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MyGameInstance.h"
#include "MySaveGame.h"

AMyPlayerState::AMyPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	SaveSlotName = TEXT("Player1");
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
	auto MySaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == MySaveGame) {
		MySaveGame = GetMutableDefault<UMySaveGame>();
	}

	SetPlayerName(MySaveGame->PlayerName);
	SetCharacterLevel(MySaveGame->Level);
	GameScore = 0;
	GameHighScore = MySaveGame->HighScore;
	Exp = MySaveGame->Exp;

	SavePlayerData();
}

void AMyPlayerState::SavePlayerData()
{
	UMySaveGame* NewPlayerData = NewObject<UMySaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0)) {
		HUNT_LOG(Error, TEXT("SaveGame Error"));
	}
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
	SavePlayerData();
	return DidLevelUp;
}

void AMyPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore) {
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void AMyPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	HUNT_CHECK(nullptr != MyGameInstance);

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewCharacterLevel);
	HUNT_CHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}

int32 AMyPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}