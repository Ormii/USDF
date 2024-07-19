// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "USDFBTDecorator_CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTDecorator_CanSeeTarget : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, Meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector AttackTargetKey;
};
