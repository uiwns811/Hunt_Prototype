// Copyright Epic Games, Inc. All Rights Reserved.


#include "Hunt_PrototypeGameModeBase.h"
#include "MyCharacter.h"

AHunt_PrototypeGameModeBase::AHunt_PrototypeGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
}