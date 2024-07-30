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

void AUSDFPlayerController::BeginSequence()
{
	if (HUDWidget)
	{
		HUDWidget->BeginSequence();
	}
}

void AUSDFPlayerController::FinishSequence()
{
	if (HUDWidget)
	{
		HUDWidget->FinishSequence();
	}
}

void AUSDFPlayerController::SetTargetBoss(AUSDFCharacterBossMonster* BossMonster)
{
	if (BossMonster == nullptr)
		return;

	TargetBoss = BossMonster;

	if (HUDWidget)
	{
		UUSDFEnemyHpBarWidget* EnemyHpBar = HUDWidget->GetBossHpBar();
		TargetBoss->SetupHpBarWidget(EnemyHpBar);
		K2_OnBossEntry();
	}
}

void AUSDFPlayerController::OnBossDead(AUSDFCharacterBossMonster* BossMonster)
{
	if (TargetBoss != nullptr && TargetBoss == BossMonster)
	{
		TargetBoss = nullptr;
		K2_OnBossExit();
	}
}
