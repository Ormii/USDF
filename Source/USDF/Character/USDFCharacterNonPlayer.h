// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBase.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "USDFCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterNonPlayer : public AUSDFCharacterBase, public IUSDFCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AUSDFCharacterNonPlayer();

	// AI Section
protected:
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)override;

	FAICharacterAttackFinished OnAttackFinished;
	virtual void AttackFinished();

};
