// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFGameOverWidget.h"
#include "Game/USDFGameMode.h"

void UUSDFGameOverWidget::K2_OnRetryButtonEvent()
{
	AUSDFGameMode* GameMode = Cast<AUSDFGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnGameRetry();
	}
}
