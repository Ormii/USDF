// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_IsHitReactState.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/USDFAICommon.h"


bool UUSDFBTDecorator_IsHitReactState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_HITREACTSTATE);
}
