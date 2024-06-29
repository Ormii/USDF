// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USDFPlayerController.h"

AUSDFPlayerController::AUSDFPlayerController()
{

}

void AUSDFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
