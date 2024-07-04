// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFPlayerHpBarWidget.h"
#include "Components/ProgressBar.h"

UUSDFPlayerHpBarWidget::UUSDFPlayerHpBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxHp = -1;
}

void UUSDFPlayerHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerHpBar")));
	ensure(HpBar);
}

void UUSDFPlayerHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0);
	CurrentHp = FMath::Clamp(NewCurrentHp, 0, MaxHp);
	HpBar->SetPercent(CurrentHp / MaxHp);
}
