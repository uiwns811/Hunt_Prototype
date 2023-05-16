// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/GameModeBase.h"
#include "Hunt_PrototypeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HUNT_PROTOTYPE_API AHunt_PrototypeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHunt_PrototypeGameModeBase();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void AddScore(class AMyPlayerController* ScoredPlayer);

	int32 GetScore() const;

private:
	UPROPERTY()
		class AMyGameState* MyGameState;

	UPROPERTY()
		int32 ScoreToClear;
};
