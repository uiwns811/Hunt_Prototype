// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "Blueprint/UserWidget.h"
#include "MyHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class HUNT_PROTOTYPE_API UMyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UMyCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AMyPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AMyPlayerState> CurrentPlayerState;

	UPROPERTY()
		class UProgressBar* HPBar;

	UPROPERTY()
		class UProgressBar* ExpBar;

	UPROPERTY()
		class UTextBlock* PlayerName;

	UPROPERTY()
		class UTextBlock* PlayerLevel;

	UPROPERTY()
		class UTextBlock* CurrentScore;

	UPROPERTY()
		class UTextBlock* HighScore;
};
