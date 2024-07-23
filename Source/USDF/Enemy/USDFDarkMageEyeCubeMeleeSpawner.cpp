// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageEyeCubeMeleeSpawner.h"
#include "Character/USDFCharacterMeleeMonster.h"

AUSDFDarkMageEyeCubeMeleeSpawner::AUSDFDarkMageEyeCubeMeleeSpawner()
{
	static ConstructorHelpers::FClassFinder<AUSDFCharacterMeleeMonster> SpawnMonsterClassRef(TEXT("/Script/USDF.USDFCharacterMeleeMonster"));
	if (SpawnMonsterClassRef.Class)
	{
		SpawnMonsterClass = SpawnMonsterClassRef.Class;
	}
}
