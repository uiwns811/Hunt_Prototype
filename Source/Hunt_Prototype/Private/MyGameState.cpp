// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

AMyGameState::AMyGameState()
{
	TotalGameScore = 0;
}

int32 AMyGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AMyGameState::AddGameScore()
{
	TotalGameScore++;
}