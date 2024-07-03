// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFEnemyHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/USDFCharacterWidgetInterface.h"

UUSDFEnemyHpBarWidget::UUSDFEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer)
	:UUSDFUserWidget(ObjectInitializer)
{
	BackgroundHp = -1;
	CurrentHp = -1;
	MaxHp = -1;
}

void UUSDFEnemyHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackgroundHpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BackgroundHpBar")));
	ensure(BackgroundHpBar);

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpBar);

	IUSDFCharacterWidgetInterface* OwningCharacter = Cast<IUSDFCharacterWidgetInterface>(Owner);
	if (OwningCharacter)
	{
		OwningCharacter->SetupHpBarWidget(this);
	}
}

void UUSDFEnemyHpBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BackgroundHp > CurrentHp)
	{
		BackgroundHp = FMath::FInterpTo(BackgroundHp, CurrentHp, InDeltaTime, 1.5f);
	}
	else
	{
		BackgroundHp = CurrentHp;
	}

	BackgroundHpBar->SetPercent(BackgroundHp/MaxHp);
}

void UUSDFEnemyHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0);
	CurrentHp = FMath::Clamp(NewCurrentHp, 0, MaxHp);
	HpBar->SetPercent(CurrentHp / MaxHp);
}
