// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "USDFBTTask_SpecialAction.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBTTask_SpecialAction : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = ActionType, Meta = (AllowPrivateAccess = "true"))
	EAIActionType ActionType;
};
