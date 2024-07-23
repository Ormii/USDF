// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageEyeCubeRangeSpawner.h"
#include "Character/USDFCharacterRangeMonster.h"

AUSDFDarkMageEyeCubeRangeSpawner::AUSDFDarkMageEyeCubeRangeSpawner()
{
	static ConstructorHelpers::FClassFinder<AUSDFCharacterRangeMonster> SpawnMonsterClassRef(TEXT("/Script/USDF.USDFCharacterRangeMonster"));
	if (SpawnMonsterClassRef.Class)
	{
		SpawnMonsterClass = SpawnMonsterClassRef.Class;
	}
}