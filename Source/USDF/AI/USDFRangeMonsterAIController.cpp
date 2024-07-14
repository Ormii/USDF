// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFRangeMonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AUSDFRangeMonsterAIController::AUSDFRangeMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/RangeMonster/BB_USDF_RangeMonster.BB_USDF_RangeMonster"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/RangeMonster/BT_USDF_RangeMonster.BT_USDF_RangeMonster"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}
