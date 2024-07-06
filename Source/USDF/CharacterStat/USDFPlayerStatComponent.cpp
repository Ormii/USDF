// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/USDFPlayerStatComponent.h"

// Sets default values for this component's properties
UUSDFPlayerStatComponent::UUSDFPlayerStatComponent()
{
	
}

void UUSDFPlayerStatComponent::InitPlayerStat(const FUSDFPlayerStat& NewPlayerStat)
{
	PlayerStat = NewPlayerStat;

	SetMaxHp(PlayerStat.MaxHp);
	SetCurrentHp(PlayerStat.MaxHp);
}
