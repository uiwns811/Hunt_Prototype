// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUDWidget.h"
#include "MyPlayerState.h"
#include "MyCharacter.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyHUDWidget> UI_HUD_C(TEXT("WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded()) 
		HUDWidgetClass = UI_HUD_C.Class;
}

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

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UMyHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	MyPlayerState = Cast<AMyPlayerState>(PlayerState);
	HUNT_CHECK(nullptr != MyPlayerState);
	HUDWidget->BindPlayerState(MyPlayerState);
	MyPlayerState->OnPlayerStateChanged.Broadcast();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

UMyHUDWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AMyPlayerController::NPCKill(AMyCharacter* KilledNPC) const
{
	MyPlayerState->AddExp(KilledNPC->GetExp());
}

void AMyPlayerController::AddGameScore() const
{
	MyPlayerState->AddGameScore();
}