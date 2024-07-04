// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTService_Detect.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/USDFAICommon.h"
#include "Physics/USDFCollision.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

UUSDFBTService_Detect::UUSDFBTService_Detect()
{
	NodeName = TEXT("Detect");
}

void UUSDFBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIContorller = OwnerComp.GetAIOwner();
	if (AIContorller == nullptr)
		return;

	APawn* Pawn = AIContorller->GetPawn();
	if (Pawn == nullptr)
		return;

	IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(Pawn);
	if (AIPawn == nullptr)
		return;

	UWorld* World = GetWorld();
	if (World == nullptr)
		return;


	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Target), false, Pawn);

	bool bIsDetected = World->OverlapMultiByChannel(OverlapResults, Pawn->GetActorLocation(), FQuat::Identity, CCHANNEL_USDF_NONPLAYERACTION
		, FCollisionShape::MakeSphere(AIPawn->GetAIDetectRadius()), Params);
	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

	if (bIsDetected)
	{
		AActor* Target = nullptr;
		for (auto& OverlapResult : OverlapResults)
		{
			if (Target == nullptr)
				Target = OverlapResult.GetActor();
			else
			{
				float TargetLength = (Pawn->GetActorLocation() - Target->GetActorLocation()).Length();
				float CompareLength = (Pawn->GetActorLocation() - OverlapResult.GetActor()->GetActorLocation()).Length();
			
				if (TargetLength > CompareLength)
					Target = OverlapResult.GetActor();
			}
		}

		if (Target)
		{
			BlackBoard->SetValueAsObject(BBKEY_TARGET, Cast<APawn>(Target));
#if ENABLE_DRAW_DEBUG
			DrawDebugSphere(World, Pawn->GetActorLocation(), AIPawn->GetAIDetectRadius(), 16, FColor::Green, false, 0.2f);
			DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
			DrawDebugLine(World, Target->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
#endif
		}
		else
			BlackBoard->SetValueAsObject(BBKEY_TARGET, nullptr);
	}
	else
	{
		BlackBoard->SetValueAsObject(BBKEY_TARGET, nullptr);

#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(World, Pawn->GetActorLocation(), AIPawn->GetAIDetectRadius(), 16, FColor::Red, false, 0.2f);
#endif
	}
}
