// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_CanSeeTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Physics/USDFCollision.h"


bool UUSDFBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return false;

	AActor* Owner = dynamic_cast<AActor*>(AIController->GetPawn());
	if (Owner == nullptr)
		return false;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (Target == nullptr)
		return false;

	FHitResult HitResult{};
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SeeTarget), false, Owner);
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActor(Owner);

	bool bHitted = GetWorld()->LineTraceSingleByChannel(HitResult, Owner->GetActorLocation(), Target->GetActorLocation(), ECC_Pawn, Params);
	if(bHitted)
		UE_LOG(LogTemp, Display, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());
	
	FColor Color = (bHitted == true) ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Target->GetActorLocation(), Color, false, 0.2f);

	return !bHitted;
}
