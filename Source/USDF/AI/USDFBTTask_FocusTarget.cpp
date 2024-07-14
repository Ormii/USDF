// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_FocusTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAIInterface.h"


EBTNodeResult::Type UUSDFBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	APawn* Target = Cast<APawn>(Blackboard->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	AIController->SetFocus(Target);

	/*
		float TurnRateSpeed = AIPawn->GetAITurnRateSpeed();
		FVector ToTargetVector = Target->GetActorLocation() - Pawn->GetActorLocation();
		ToTargetVector.Z = 0;
		FRotator Rotation = FRotationMatrix::MakeFromX(ToTargetVector).Rotator();

		Pawn->SetActorRotation(FMath::RInterpTo(Pawn->GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), TurnRateSpeed));
		Pawn->SetActorRotation(Rotation);
	*/
	return EBTNodeResult::Succeeded;
}
