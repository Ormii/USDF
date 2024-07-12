// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFAIMeleeMonsterSpawner.h"
#include "Character/USDFCharacterMeleeMonster.h"

AUSDFAIMeleeMonsterSpawner::AUSDFAIMeleeMonsterSpawner()
{
	static ConstructorHelpers::FClassFinder<AUSDFCharacterMeleeMonster> SpawnMonsterClassRef(TEXT("/Script/USDF.USDFCharacterMeleeMonster"));
	if (SpawnMonsterClassRef.Class)
	{
		SpawnMonsterClass = SpawnMonsterClassRef.Class;
	}
}
