// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/USDFCharacterStatComponent.h"

// Sets default values for this component's properties
UUSDFCharacterStatComponent::UUSDFCharacterStatComponent()
{
	MaxHp = 200;
	CurrentHp = MaxHp;
}

void UUSDFCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetCurrentHp(MaxHp);
}

void UUSDFCharacterStatComponent::SetCurrentHp(float NewCurrentHp)
{
	float ActualCurrentHp = FMath::Clamp(NewCurrentHp, 0, MaxHp);
	CurrentHp = ActualCurrentHp;

	OnHpChangedDelegate.Broadcast(NewCurrentHp);
}

void UUSDFCharacterStatComponent::ApplyDamage(float DamageAmount)
{
	float NewCurrentHp = FMath::Clamp(CurrentHp - DamageAmount, 0, MaxHp);
	SetCurrentHp(NewCurrentHp);
}
