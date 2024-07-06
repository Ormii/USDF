// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/USDFNormalMonsterStatComponent.h"
#include "GameData/USDFNormalMonsterStat.h"

// Sets default values for this component's properties
UUSDFNormalMonsterStatComponent::UUSDFNormalMonsterStatComponent()
{

}

void UUSDFNormalMonsterStatComponent::InitNormalMonsterStat(const FUSDFNormalMonsterStat& NewNormalMonsterStat)
{
	NormalMonsterStat = NewNormalMonsterStat;
	
	SetMaxHp(NormalMonsterStat.MaxHp);
	SetCurrentHp(NormalMonsterStat.MaxHp);
}

