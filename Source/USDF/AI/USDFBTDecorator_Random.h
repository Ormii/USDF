// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "USDFBTDecorator_Random.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTDecorator_Random : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UUSDFBTDecorator_Random();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = Random)
	float Probability;
};
