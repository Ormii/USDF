// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/USDFAIController.h"
#include "USDFBossDarkMageAIController.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFBossDarkMageAIController : public AUSDFAIController
{
	GENERATED_BODY()
	
public:
	AUSDFBossDarkMageAIController();

public:
	void SetNormalMonsterCount(int32 NewCount);
};
