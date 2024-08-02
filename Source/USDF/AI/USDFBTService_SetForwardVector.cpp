// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTService_SetForwardVector.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "USDFAIController.h"
#include "AI/USDFAICommon.h"

void UUSDFBTService_SetForwardVector::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return;


	AUSDFAIController* AIController = Cast<AUSDFAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return;

	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
		return;

	Blackboard->SetValueAsVector(BBKEY_FORWARD_VECTOR, Pawn->GetActorForwardVector());

}
