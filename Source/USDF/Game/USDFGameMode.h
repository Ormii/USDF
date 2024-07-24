// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"
#include "USDFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFGameMode : public AGameModeBase, public IUSDFGameModeInterface
{
	GENERATED_BODY()
	
public:
	AUSDFGameMode();

	// GameMode Section
public:
	virtual void OnPlayerDead() override;
	virtual void OnBossEntry(class AUSDFCharacterBossMonster* BossMonster) override;
	virtual void OnBossDead(class AUSDFCharacterBossMonster* BossMonster) override;


	virtual void OnGameRetry() override;
};
