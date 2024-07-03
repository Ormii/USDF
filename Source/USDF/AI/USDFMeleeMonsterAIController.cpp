// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFMeleeMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/USDFAICommon.h"

AUSDFMeleeMonsterAIController::AUSDFMeleeMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/BB_USDF_MeleeMonster.BB_USDF_MeleeMonster"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/BT_USDF_MeleeMonster.BT_USDF_MeleeMonster"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
	
}
