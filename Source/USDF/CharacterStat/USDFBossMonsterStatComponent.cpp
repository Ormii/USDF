// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/USDFBossMonsterStatComponent.h"
#include "GameData/USDFBossMonsterStat.h"

UUSDFBossMonsterStatComponent::UUSDFBossMonsterStatComponent()
{
}

void UUSDFBossMonsterStatComponent::InitBossMonsterStat(const FUSDFBossMonsterStat& NewBossMonsterStat)
{
	BossMonsterStat = NewBossMonsterStat;

	SetMaxHp(BossMonsterStat.MaxHp);
	SetCurrentHp(BossMonsterStat.MaxHp);
}
