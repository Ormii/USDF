// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_IsHealthBelow.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "Interface/USDFDamageableInterface.h"
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

	IUSDFDamageableInterface* DamageablePawn = Cast<IUSDFDamageableInterface>(AIController->GetPawn());
	if (DamageablePawn == nullptr)
		return false;

	float MaxHealth = DamageablePawn->GetMaxHealth();
	float CurrentHealth = DamageablePawn->GetCurrentHealth();

	float Ratio = CurrentHealth / MaxHealth;

	
	return Ratio <= HealthThreshold;
}
