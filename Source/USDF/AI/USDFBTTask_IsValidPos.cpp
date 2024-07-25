// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_IsValidPos.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AIController.h"


EBTNodeResult::Type UUSDFBTTask_IsValidPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FVector TargetPos = Blackboard->GetValueAsVector(TargetPosKey.SelectedKeyName);
	UNavigationSystemV1* Nav = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (Nav == nullptr)
		return EBTNodeResult::Failed;

	UNavigationPath* NavPath =Nav->FindPathToLocationSynchronously(Pawn, Pawn->GetActorLocation(), TargetPos); 
	if (NavPath == nullptr)
		return EBTNodeResult::Failed;

	if (!NavPath->IsValid())
		return EBTNodeResult::Failed;
	
	return EBTNodeResult::Type::Succeeded;
}
