// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFAIRangeMonsterSpawner.h"
#include "Character/USDFCharacterRangeMonster.h"

AUSDFAIRangeMonsterSpawner::AUSDFAIRangeMonsterSpawner()
{
	static ConstructorHelpers::FClassFinder<AUSDFCharacterRangeMonster> SpawnMonsterClassRef(TEXT("/Script/USDF.USDFCharacterRangeMonster"));
	if (SpawnMonsterClassRef.Class)
	{
		SpawnMonsterClass = SpawnMonsterClassRef.Class;
	}
}
