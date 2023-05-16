// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUDWidget.h"
#include "MyPlayerState.h"
#include "MyCharacter.h"
#include "MyGameplayWidget.h"
#include "MyGameplayResultWidget.h"
#include "MyGameState.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyHUDWidget> UI_HUD_C(TEXT("WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded()) 
		HUDWidgetClass = UI_HUD_C.Class;

	static ConstructorHelpers::FClassFinder<UMyGameplayWidget> UI_MENU_C(TEXT("WidgetBlueprint'/Game/UI/UI_Menu.UI_Menu_C'"));
	if (UI_MENU_C.Succeeded())
		MenuWidgetClass = UI_MENU_C.Class;

	static ConstructorHelpers::FClassFinder<UMyGameplayResultWidget> UI_RESULT_C(TEXT("WidgetBlueprint'/Game/UI/UI_Result.UI_Result_C'"));
	if (UI_RESULT_C.Succeeded())
		ResultWidgetClass = UI_RESULT_C.Class;

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

	ChangeInputMode(true);

	HUDWidget = CreateWidget<UMyHUDWidget>(this, HUDWidgetClass);
	HUNT_CHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<UMyGameplayResultWidget>(this, ResultWidgetClass);
	HUNT_CHECK(nullptr != ResultWidget);

	MyPlayerState = Cast<AMyPlayerState>(PlayerState);
	HUNT_CHECK(nullptr != MyPlayerState);
	HUDWidget->BindPlayerState(MyPlayerState);
	MyPlayerState->OnPlayerStateChanged.Broadcast();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AMyPlayerController::OnGamePause);
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

void AMyPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UMyGameplayWidget>(this, MenuWidgetClass);
	HUNT_CHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

void AMyPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode) {
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else {
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AMyPlayerController::ShowResultUI()
{
	auto MyGameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(this));
	HUNT_CHECK(nullptr != MyGameState);
	ResultWidget->BindGameState(MyGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}