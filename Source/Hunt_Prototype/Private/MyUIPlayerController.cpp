// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUNT_CHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	HUNT_CHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}