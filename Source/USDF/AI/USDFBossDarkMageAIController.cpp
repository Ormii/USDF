// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBossDarkMageAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/USDFAICommon.h"

AUSDFBossDarkMageAIController::AUSDFBossDarkMageAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/AI/BossDarkMage/Default/BB_USDF_BossDarkMage_Default.BB_USDF_BossDarkMage_Default"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/AI/BossDarkMage/Default/BT_USDF_Boss_DarkMage_Default.BT_USDF_Boss_DarkMage_Default"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}