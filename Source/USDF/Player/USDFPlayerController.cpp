// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USDFPlayerController.h"
#include "UI/USDFPlayerHUDWidget.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "Character/USDFCharacterBossMonster.h"

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

void AUSDFPlayerController::SetTargetBoss(AUSDFCharacterBossMonster* BossMonster)
{
	if (TargetBoss == BossMonster)
		return;

	TargetBoss = BossMonster;

	if (HUDWidget)
	{
		UUSDFEnemyHpBarWidget* EnemyHpBar = HUDWidget->GetBossHpBar();
		if (TargetBoss == nullptr)
		{
			EnemyHpBar->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TargetBoss->SetupHpBarWidget(EnemyHpBar);
			EnemyHpBar->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AUSDFPlayerController::OnBossDead(AUSDFCharacterBossMonster* BossMonster)
{
	if (TargetBoss != nullptr && TargetBoss == BossMonster)
	{
		SetTargetBoss(nullptr);
	}
}
