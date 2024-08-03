// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_IncreaseSearchCount.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "USDFAIController.h"
#include "USDFAICommon.h"

EBTNodeResult::Type UUSDFBTTask_IncreaseSearchCount::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	int32 SearchCount = Blackboard->GetValueAsInt(BBKEY_SEARCH_COUNT);
	Blackboard->SetValueAsInt(BBKEY_SEARCH_COUNT, SearchCount + 1);

    return EBTNodeResult::Succeeded;
}
