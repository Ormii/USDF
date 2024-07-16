// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_MoveAlongPatrolRoute.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "USDFPatrolRoute.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utility/LambdaWrapper.h"
#include "Animation/USDFLocomotionState.h"

void UUSDFBTTask_MoveAlongPatrolRoute::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	LambdaWrapper = NewObject<ULambdaWrapper>();
}

EBTNodeResult::Type UUSDFBTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(Pawn);
	if (AIPawn == nullptr)
		return EBTNodeResult::Failed;

	AUSDFPatrolRoute* PatrolRoute = AIPawn->GetPatrolRoute();
	if (PatrolRoute)
	{
		FVector Location =  PatrolRoute->GetSplinePointAsWorldPosition();
		FAIMoveRequest MoveRequest{};
		MoveRequest.SetGoalLocation(Location);
		auto Result = AIController->MoveTo(MoveRequest);

		if (LambdaWrapper->OnMoveCompleteCallFn == nullptr)
		{
			LambdaWrapper->OnMoveCompleteCallFn = [&](FAIRequestID RequestID, EPathFollowingResult::Type Result) {

				AIPawn = Cast<IUSDFCharacterAIInterface>(OwnerComp.GetAIOwner()->GetPawn());
				if (AIPawn == nullptr)
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

				AUSDFPatrolRoute* Route = AIPawn->GetPatrolRoute();
				if (Route == nullptr)
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

				Route->IncrementPatrolRoute();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				};
			AIController->ReceiveMoveCompleted.AddDynamic(LambdaWrapper, &ULambdaWrapper::OnMoveCompleteDispatch);
		}
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UUSDFBTTask_MoveAlongPatrolRoute::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	AIController->StopMovement();

	return EBTNodeResult::Aborted;
}
