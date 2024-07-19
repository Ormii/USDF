// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFMeleeMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/USDFAICommon.h"

AUSDFMeleeMonsterAIController::AUSDFMeleeMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/MeleeMonster/Default/BB_USDFMeleeDefault.BB_USDFMeleeDefault"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/MeleeMonster/Default/BT_USDFMeleeDefault.BT_USDFMeleeDefault"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
	
}
