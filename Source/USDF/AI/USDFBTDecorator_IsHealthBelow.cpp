// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_IsHealthBelow.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "AI/USDFAICommon.h"

bool UUSDFBTDecorator_IsHealthBelow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	float MaxHealth = AIPawn->GetMaxHealth();
	float CurrentHealth = AIPawn->GetCurrentHealth();

	float Ratio = CurrentHealth / MaxHealth;

	
	return Ratio <= HealthThreshold;
}
