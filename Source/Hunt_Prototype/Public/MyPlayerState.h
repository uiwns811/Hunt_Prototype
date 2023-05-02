// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class HUNT_PROTOTYPE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMyPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	void AddGameScore();

	void InitPlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
		int32 GameScore;

	UPROPERTY(Transient)
		int32 CharacterLevel;
	
	UPROPERTY(Transient)
		int32 Exp;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FMyCharacterData* CurrentStatData;
};
