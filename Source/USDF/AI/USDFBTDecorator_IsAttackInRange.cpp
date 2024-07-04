// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_IsAttackInRange.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "AI/USDFAICommon.h"
#include "DrawDebugHelpers.h"

bool UUSDFBTDecorator_IsAttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return false;

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(AIController->GetPawn());
	if (AIPawn == nullptr)
		return false;

	APawn* Target = Cast<APawn>(Blackboard->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
		return false;

	float TargetLength = (AIController->GetPawn()->GetActorLocation() - Target->GetActorLocation()).Length();

	if (TargetLength <= AIPawn->GetAIAttackRange())
	{
#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(GetWorld(), AIController->GetPawn()->GetActorLocation(), AIPawn->GetAIAttackRange(), 10, FColor::Green, false, 0.2f);
#endif
		return true;
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), AIController->GetPawn()->GetActorLocation(), AIPawn->GetAIAttackRange(), 10, FColor::Red, false, 0.2f);
#endif

	return false;
}
