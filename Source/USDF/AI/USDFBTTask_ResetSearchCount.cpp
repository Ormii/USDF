// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_ResetSearchCount.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "USDFAIController.h"
#include "USDFAICommon.h"

EBTNodeResult::Type UUSDFBTTask_ResetSearchCount::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	Blackboard->SetValueAsInt(BBKEY_SEARCH_COUNT, 0);

	return EBTNodeResult::Succeeded;
}
