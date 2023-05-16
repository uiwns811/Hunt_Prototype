// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HUNT_PROTOTYPE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMySaveGame();

	UPROPERTY()
		int32 Level;

	UPROPERTY()
		int32 Exp;

	UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		int32 HighScore;

	UPROPERTY()
		int32 CharacterIndex;
};
