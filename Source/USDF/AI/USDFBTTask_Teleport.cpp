// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTTask_Teleport.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "Utility/LambdaWrapper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UUSDFBTTask_Teleport::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

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

	ACharacter* Pawn = Cast<ACharacter>(AIController->GetPawn());
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	IUSDFCharacterAttackAnimInterface* CharacterPawn = Cast<IUSDFCharacterAttackAnimInterface>(Pawn);
	if (CharacterPawn == nullptr)
		return EBTNodeResult::Failed;

	FVector TargetLocation = Blackboard->GetValueAsVector(TeleportLocationKey.SelectedKeyName);
	CharacterPawn->TeleportStart();
	float Radius = Pawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float HalfHeight = Pawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Pawn->SetActorLocation(TargetLocation + FVector(0.0f, 0.0f, Radius + HalfHeight));
	CharacterPawn->TeleportEnd();

	return EBTNodeResult::Succeeded;
}