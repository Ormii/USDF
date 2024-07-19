// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "USDFBTTask_MoveTo.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTTask_MoveTo : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, Meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector Key;

protected:
	UPROPERTY()
	mutable TObjectPtr<class ULambdaWrapper> LambdaWrapper;
};
