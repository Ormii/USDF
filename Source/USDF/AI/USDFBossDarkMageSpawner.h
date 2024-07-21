// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/USDFAISpawner.h"
#include "USDFBossDarkMageSpawner.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFBossDarkMageSpawner : public AUSDFAISpawner
{
	GENERATED_BODY()

public:
	AUSDFBossDarkMageSpawner();

protected:
	virtual void Spawn(const FStageSpawnOrder& SpawnOrder) override;
};
