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
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	void RunAI();
	void StopAI();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;

};
