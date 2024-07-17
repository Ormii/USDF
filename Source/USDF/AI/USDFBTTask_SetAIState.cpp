// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_SetAIState.h"
#include "USDFAICommon.h"
#include "USDFAIController.h"

EBTNodeResult::Type UUSDFBTTask_SetAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AUSDFAIController* AIController = Cast<AUSDFAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	AIController->SetCurrentAIState(AIState, FAISensedParam());

	return EBTNodeResult::Succeeded;
}
