// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "USDFBTDecorator_IsInIdealrange.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTDecorator_IsInIdealrange : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
