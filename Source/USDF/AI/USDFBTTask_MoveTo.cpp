// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_MoveTo.h"
#include "AI/USDFAIController.h"

EBTNodeResult::Type UUSDFBTTask_MoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	
	AUSDFAIController* AIController = Cast<AUSDFAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return EBTNodeResult::Failed;
	
	AIController->StopMovement();

	return EBTNodeResult::Aborted;
}
