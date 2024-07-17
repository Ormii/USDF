// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_IsInIdealrange.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "USDFAIController.h"
#include "AI/USDFAICommon.h"

bool UUSDFBTDecorator_IsInIdealrange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return false;

	AUSDFAIController* AIController = Cast<AUSDFAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return false;

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(AIController->GetPawn());
	if (AIPawn == nullptr)
		return false;

	AActor* AttackTarget = Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_ATTACK_TARGET));

	if (AttackTarget)
	{
		float Distance = AttackTarget->GetDistanceTo(AIController->GetPawn());
		float EQSDefendRange = AIPawn->GetAIEQSDefendRadius();

		return (Distance - 50) <= EQSDefendRange;
	}

	return false;
}
