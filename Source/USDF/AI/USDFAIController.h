// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "USDFAIController.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AUSDFAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void RunAI();
	void StopAI();
protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
