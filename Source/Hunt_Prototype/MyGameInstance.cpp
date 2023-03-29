// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/GameData/Hunt_Prototype_11.Hunt_Prototype_11'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(*CharacterDataPath);
	HUNT_CHECK(DT_MYCHARACTER.Succeeded());
	MyCharacterTable = DT_MYCHARACTER.Object;
	HUNT_CHECK(MyCharacterTable->GetRowMap().Num() > 0);
}

void UMyGameInstance::Init()
{
	Super::Init();
	HUNT_LOG(Warning, TEXT("DropExp of Level 20 MyCharacter : %d"), GetMyCharacterData(20)->DropExp);
}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}