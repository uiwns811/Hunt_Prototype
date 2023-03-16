// Copyright Epic Games, Inc. All Rights Reserved.


#include "Hunt_PrototypeGameModeBase.h"
#include "MyPawn.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AHunt_PrototypeGameModeBase::AHunt_PrototypeGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

void AHunt_PrototypeGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}