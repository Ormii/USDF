// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_GetPatrolRoute.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "AIController.h"

bool UUSDFBTDecorator_GetPatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return false;

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(AIController->GetPawn());
	if (AIPawn == nullptr)
		return false;

	return AIPawn->GetPatrolRoute() != nullptr;
}
