// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "USDFBTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UUSDFBTService_Detect();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
