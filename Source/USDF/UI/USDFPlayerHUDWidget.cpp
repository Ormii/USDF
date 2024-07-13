// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFPlayerHUDWidget.h"
#include "UI/USDFPlayerHpBarWidget.h"
#include "UI/USDFPrepareStageWidget.h"
#include "UI/USDFEndStageWidget.h"
#include "Interface/USDFCharacterPlayerHUDInterface.h"

UUSDFPlayerHUDWidget::UUSDFPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UUSDFPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerHpBar = Cast<UUSDFPlayerHpBarWidget>(GetWidgetFromName(TEXT("PlayerHpBar")));
	ensure(PlayerHpBar);

	PrepareStageWidget = Cast<UUSDFPrepareStageWidget>(GetWidgetFromName(TEXT("PrepareStage")));
	ensure(PrepareStageWidget);

	EndStageWidget = Cast<UUSDFEndStageWidget>(GetWidgetFromName(TEXT("EndStage")));
	ensure(EndStageWidget);
}

void UUSDFPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IUSDFCharacterPlayerHUDInterface* HUDWidgetInterface = Cast<IUSDFCharacterPlayerHUDInterface>(GetOwningPlayerPawn());
	if (HUDWidgetInterface)
	{
		HUDWidgetInterface->SetupPlayerHpBarHUDWidget(PlayerHpBar);
	}
}