// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFRangeMonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AUSDFRangeMonsterAIController::AUSDFRangeMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/RangeMonster/Evad/BB_USDFRangeEvad.BB_USDFRangeEvad"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/RangeMonster/Evad/BT_USDFRangeEvad.BT_USDFRangeEvad"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AUSDFRangeMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetCurrentAIState(EAIState::Investigating, FAISensedParam{});
}
