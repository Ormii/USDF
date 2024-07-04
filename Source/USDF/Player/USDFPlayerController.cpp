// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USDFPlayerController.h"
#include "UI/USDFPlayerHUDWidget.h"

AUSDFPlayerController::AUSDFPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUSDFPlayerHUDWidget> HUDWidgetClassRef(TEXT("/Game/Blueprint/Widget/BP_USDFPlayerHUDWidget.BP_USDFPlayerHUDWidget_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
}

void AUSDFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
