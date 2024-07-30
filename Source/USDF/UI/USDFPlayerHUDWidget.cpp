// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFPlayerHUDWidget.h"
#include "UI/USDFPlayerHpBarWidget.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "UI/USDFGameStageIntroWidget.h"
#include "Interface/USDFCharacterPlayerHUDInterface.h"
#include "Player/USDFPlayerController.h"

UUSDFPlayerHUDWidget::UUSDFPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UUSDFPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerHpBar = Cast<UUSDFPlayerHpBarWidget>(GetWidgetFromName(TEXT("PlayerHpBar")));
	ensure(PlayerHpBar);

	BossHpBar = Cast<UUSDFEnemyHpBarWidget>(GetWidgetFromName(TEXT("BossHpBar")));
	ensure(BossHpBar);
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

void UUSDFPlayerHUDWidget::BeginSequence()
{
	PlayerHpBar->SetVisibility(ESlateVisibility::Hidden);
	BossHpBar->SetVisibility(ESlateVisibility::Hidden);
}

void UUSDFPlayerHUDWidget::FinishSequence()
{
	PlayerHpBar->SetVisibility(ESlateVisibility::Visible);
	
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		if (PlayerController->GetTargetBoss() != nullptr)
			BossHpBar->SetVisibility(ESlateVisibility::Visible);
	}
}
