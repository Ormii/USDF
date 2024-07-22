// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_CanSeeTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Physics/USDFCollision.h"
#include "GameFramework/Character.h"

bool UUSDFBTDecorator_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return false;

	ACharacter* Owner = Cast<ACharacter>(AIController->GetPawn());
	if (Owner == nullptr)
		return false;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (Target == nullptr)
		return false;

	FHitResult HitResult{};
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SeeTarget), false, Owner);
	Params.AddIgnoredActor(Target);
	Params.AddIgnoredActor(Owner);

	FVector StartLocation = Owner->GetMesh()->GetSocketLocation("eye_socket");

	bool bHitted = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, Target->GetActorLocation(), CCHANNEL_USDF_NON_PLAYER_CHARACTER, Params);
	if(bHitted)
		UE_LOG(LogTemp, Display, TEXT("HitResult : %s"), *HitResult.GetActor()->GetName());
	
	FColor Color = (bHitted == true) ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), StartLocation, Target->GetActorLocation(), Color, false, 0.2f);

	return !bHitted;
}
