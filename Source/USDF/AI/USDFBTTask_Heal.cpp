// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_Heal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAIInterface.h"

EBTNodeResult::Type UUSDFBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;


	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(Pawn);
	if (AIPawn == nullptr)
		return EBTNodeResult::Failed;

	
	HealPercentage = FMath::Clamp(HealPercentage, 0.0f, 1.0f);
	float HealAmount = AIPawn->GetCurrentHealth() * HealPercentage;
	AIPawn->Heal(HealAmount);

	return EBTNodeResult::Succeeded;
}
