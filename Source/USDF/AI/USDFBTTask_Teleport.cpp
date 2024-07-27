// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_Teleport.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "Utility/LambdaWrapper.h"
#include "Kismet/KismetSystemLibrary.h"

void UUSDFBTTask_Teleport::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

	LambdaWrapper = NewObject<ULambdaWrapper>();
}

EBTNodeResult::Type UUSDFBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FVector TargetLocation = Blackboard->GetValueAsVector(TeleportLocationKey.SelectedKeyName);
	FAIMoveRequest MoveRequest{};
	MoveRequest.SetAcceptanceRadius(20.0f);
	MoveRequest.SetGoalLocation(TargetLocation);

	CharacterPawn->TeleportStart();
	auto Result = AIController->MoveTo(MoveRequest);
	
	if (LambdaWrapper->OnMoveCompleteCallFn == nullptr)
	{
		LambdaWrapper->OnMoveCompleteCallFn = [&](FAIRequestID RequestID, EPathFollowingResult::Type Result) {

				AIController = OwnerComp.GetAIOwner();
				if (AIController == nullptr)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					return;
				}

				Pawn = Cast<APawn>(AIController->GetPawn());
				if (Pawn == nullptr)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					return;
				}

				CharacterPawn = Cast<IUSDFCharacterAttackAnimInterface>(Pawn);
				if (CharacterPawn == nullptr)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					return;
				}

				CharacterPawn->TeleportEnd();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				AIController->ReceiveMoveCompleted.Clear();

		};
		AIController->ReceiveMoveCompleted.AddDynamic(LambdaWrapper, &ULambdaWrapper::OnMoveCompleteDispatch);
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UUSDFBTTask_Teleport::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawn* Pawn = Cast<APawn>(AIController->GetPawn());
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	IUSDFCharacterAttackAnimInterface* CharacterPawn = Cast<IUSDFCharacterAttackAnimInterface>(Pawn);
	if (CharacterPawn == nullptr)
		return EBTNodeResult::Failed;

	CharacterPawn->TeleportEnd();
	AIController->StopMovement();


	return EBTNodeResult::Aborted;
}
