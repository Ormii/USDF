// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_MoveTo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Utility/LambdaWrapper.h"

void UUSDFBTTask_MoveTo::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	LambdaWrapper = NewObject<ULambdaWrapper>();
}

EBTNodeResult::Type UUSDFBTTask_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FVector MovePoint = Blackboard->GetValueAsVector(Key.SelectedKeyName);


	FAIMoveRequest MoveRequest{};
	MoveRequest.SetGoalLocation(MovePoint);
	MoveRequest.SetAcceptanceRadius(10.0f);
	auto Result = AIController->MoveTo(MoveRequest);
	
	if (LambdaWrapper->OnMoveCompleteCallFn == nullptr)
	{
		LambdaWrapper->OnMoveCompleteCallFn = [&](FAIRequestID RequestID, EPathFollowingResult::Type Result) {

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		};
		AIController->ReceiveMoveCompleted.AddDynamic(LambdaWrapper, &ULambdaWrapper::OnMoveCompleteDispatch);
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UUSDFBTTask_MoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	AIController->StopMovement();

	return EBTNodeResult::Aborted;
}
