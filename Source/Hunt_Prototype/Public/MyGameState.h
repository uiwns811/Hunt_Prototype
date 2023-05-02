// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class HUNT_PROTOTYPE_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

private:
	UPROPERTY(Transient)
		int32 TotalGameScore;
};
