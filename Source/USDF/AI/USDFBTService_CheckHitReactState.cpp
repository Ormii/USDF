// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTService_CheckHitReactState.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/USDFAICommon.h"
#include "Interface/USDFCharacterHitReactInterface.h"

UUSDFBTService_CheckHitReactState::UUSDFBTService_CheckHitReactState()
{
	NodeName = TEXT("CheckHitReactState");
}

void UUSDFBTService_CheckHitReactState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	IUSDFCharacterHitReactInterface* HitReactPawn = Cast<IUSDFCharacterHitReactInterface>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (HitReactPawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_HITREACTSTATE, HitReactPawn->GetHitReactState());
	}
}
