// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_RangeAttack.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "AIController.h"

EBTNodeResult::Type UUSDFBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FAICharacterActionFinished OnActionFinished;
	OnActionFinished.BindLambda([&]() {

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	AIPawn->SetAIActionDelegate(OnActionFinished);
	AIPawn->ActionByAI(EAIActionType::Range);

	return EBTNodeResult::InProgress;
}
