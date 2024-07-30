// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFGameStageEndingWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"

void UUSDFGameStageEndingWidget::K2_OnStageEndingRetryButtonEvent()
{
	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnGameRetry();
	}
}
