// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "USDFBTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTTask_Heal : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, Meta = (AllowPrivateAccess = "true"))
	float HealPercentage;
};
