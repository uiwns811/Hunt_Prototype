// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hunt_Prototype.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class HUNT_PROTOTYPE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UMyHUDWidget* GetHUDWidget() const;
	void NPCKill(class AMyCharacter* KilledNPC) const;
	void AddGameScore() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UMyHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
		class UMyHUDWidget* HUDWidget;

	UPROPERTY()
		class AMyPlayerState* MyPlayerState;
};
