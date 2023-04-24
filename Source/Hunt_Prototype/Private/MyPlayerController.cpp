// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HUNT_LOG_S(Warning);
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	HUNT_LOG_S(Warning);
	Super::OnPossess(aPawn);
}