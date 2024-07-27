// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_TeleportStart.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"

EBTNodeResult::Type UUSDFBTTask_TeleportStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawn* Pawn = Cast<APawn>(AIController->GetPawn());
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	IUSDFCharacterAttackAnimInterface* CharacterPawn = Cast<IUSDFCharacterAttackAnimInterface>(Pawn);
	if (CharacterPawn == nullptr)
		return EBTNodeResult::Failed;

	CharacterPawn->TeleportStart();

	return EBTNodeResult::Succeeded;
}
