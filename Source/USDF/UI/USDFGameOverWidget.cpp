// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFGameOverWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"

void UUSDFGameOverWidget::K2_OnGameOverRetryButtonEvent()
{
	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnGameRetry();
	}
}
