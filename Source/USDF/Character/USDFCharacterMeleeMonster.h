// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNormalMonster.h"
#include "USDFCharacterMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterMeleeMonster : public AUSDFCharacterNormalMonster
{
	GENERATED_BODY()

public:
	AUSDFCharacterMeleeMonster();

	// Dead Section
protected:
	virtual void SetDead() override;

	// AI Section
protected:
	virtual float GetAIPatrolRadius() { return 600.0f; };
};
