// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Animation/USDFLocomotionState.h"
#include "USDFBTTask_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Locomotion, Meta = (AllowPrivateAccess="true"))
	ELocomotionState LocomotionState;
};
