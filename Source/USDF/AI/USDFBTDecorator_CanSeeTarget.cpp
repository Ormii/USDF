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
	FVector StartLocation = Owner->GetMesh()->GetSocketLocation("eye_socket");
	FVector EndLocation = Target->GetActorLocation();
	float Radius = 40.0f;

	bool bHitted = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, CCHANNEL_USDF_ENEMY_PROJECTILE, FCollisionShape::MakeSphere(Radius), Params);
	bool bTargetHit = true;
	if (bHitted)
	{
		if (Target == HitResult.GetActor())
			bTargetHit = true;
		else
		{ 
			bTargetHit = false;
			UE_LOG(LogTemp, Display, TEXT("Hit Target Name: %s"), *HitResult.GetActor()->GetName());
		}
	}
	else
	{
		bTargetHit = false;
	}

#if ENABLE_DRAW_DEBUG
	/*
	FColor Color = (bTargetHit == true) ? FColor::Green : FColor::Red;
	FVector TargetFowardVector = EndLocation - StartLocation;
	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2, TargetFowardVector.Length() * 0.5f, Radius, FRotationMatrix::MakeFromZ(TargetFowardVector).ToQuat(), Color, false, 0.1f);
	*/
#endif

	return bTargetHit;
}
