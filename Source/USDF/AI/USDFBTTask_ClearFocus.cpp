// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_ClearFocus.h"
#include "AIController.h"

EBTNodeResult::Type UUSDFBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);

	return EBTNodeResult::Succeeded;
}
