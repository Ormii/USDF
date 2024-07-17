// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_MoveToTargetRange.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "Utility/LambdaWrapper.h"

void UUSDFBTTask_MoveToTargetRange::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	LambdaWrapper = NewObject<ULambdaWrapper>();
}

EBTNodeResult::Type UUSDFBTTask_MoveToTargetRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AActor* AttackTarget = Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_ATTACK_TARGET));
	if (AttackTarget == nullptr)
		return EBTNodeResult::Failed;

	float EQSDefendRange = AIPawn->GetAIEQSDefendRadius();

	FAIMoveRequest MoveRequest{};
	MoveRequest.SetGoalLocation(AttackTarget->GetActorLocation());
	MoveRequest.SetAcceptanceRadius(EQSDefendRange);
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

EBTNodeResult::Type UUSDFBTTask_MoveToTargetRange::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	Super::AbortTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	AIController->StopMovement();

	return EBTNodeResult::Aborted;
}
