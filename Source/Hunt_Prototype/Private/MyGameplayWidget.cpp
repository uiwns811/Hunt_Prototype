// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayWidget.h"
#include "Components/Button.h"
#include "MyPlayerController.h"

void UMyGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton) {
		ResumeButton->OnClicked.AddDynamic(this, &UMyGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton) {
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UMyGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton) {
		RetryGameButton->OnClicked.AddDynamic(this, &UMyGameplayWidget::OnRetryGameClicked);
	}
}

void UMyGameplayWidget::OnResumeClicked()
{
	auto MyPlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	HUNT_CHECK(nullptr != MyPlayerController);

	RemoveFromParent();

	MyPlayerController->ChangeInputMode(true);
	MyPlayerController->SetPause(false);
}

void UMyGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UMyGameplayWidget::OnRetryGameClicked()
{
	auto MyPlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	HUNT_CHECK(nullptr != MyPlayerController);
	MyPlayerController->RestartLevel();
}