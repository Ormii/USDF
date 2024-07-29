// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/USDFAIController.h"
#include "USDFMeleeMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFMeleeMonsterAIController : public AUSDFAIController
{
	GENERATED_BODY()
	
public:
	AUSDFMeleeMonsterAIController();

protected:
	void OnPossess(APawn* InPawn) override;
};
