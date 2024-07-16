// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_SetMovementSpeed.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "AIController.h"
#include "Animation/USDFLocomotionState.h"

EBTNodeResult::Type UUSDFBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawn* Pawn = Cast<APawn>(AIController->GetPawn());
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(Pawn);
	if (AIPawn == nullptr)
		return EBTNodeResult::Failed;

	AIPawn->SetLocomotionState(LocomotionState);


	return EBTNodeResult::Succeeded;
}
