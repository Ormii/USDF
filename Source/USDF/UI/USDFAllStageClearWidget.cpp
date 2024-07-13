// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFAllStageClearWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"

void UUSDFAllStageClearWidget::K2_OnAllStageClearRetryButtonEvent()
{
	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnGameRetry();
	}
}
