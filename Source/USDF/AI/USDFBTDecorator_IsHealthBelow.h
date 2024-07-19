// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "USDFBTDecorator_IsHealthBelow.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTDecorator_IsHealthBelow : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, Meta = (AllowPrivateAccess = "true"))
	float HealthThreshold;
};
