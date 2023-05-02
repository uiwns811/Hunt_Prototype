// Copyright Epic Games, Inc. All Rights Reserved.


#include "Hunt_PrototypeGameModeBase.h"
#include "MyPawn.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "MyGameState.h"

AHunt_PrototypeGameModeBase::AHunt_PrototypeGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}

void AHunt_PrototypeGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyGameState = Cast<AMyGameState>(GameState);
}

void AHunt_PrototypeGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto MyPlayerState = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	HUNT_CHECK(nullptr != MyPlayerState);
	MyPlayerState->InitPlayerData();
}

void AHunt_PrototypeGameModeBase::AddScore(AMyPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		const auto MyPlayerController = Cast<AMyPlayerController>(It->Get());
		if (nullptr != MyPlayerController && ScoredPlayer == MyPlayerController) {
			MyPlayerController->AddGameScore();
			break;
		}
	}
	MyGameState->AddGameScore();
}