// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "USDFBTTask_MoveTo.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTTask_MoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
