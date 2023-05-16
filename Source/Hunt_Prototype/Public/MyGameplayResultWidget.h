// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "MyGameplayWidget.h"
#include "MyGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class HUNT_PROTOTYPE_API UMyGameplayResultWidget : public UMyGameplayWidget
{
	GENERATED_BODY()

public:
	void BindGameState(class AMyGameState* GameState);
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class AMyGameState> CurrentGameState;
};
