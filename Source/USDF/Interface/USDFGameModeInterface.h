// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USDFGameModeInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGoalComplete);

USTRUCT()
struct FGameModeGoalParam
{
	GENERATED_BODY()

	FGameModeGoalParam() {}
	FGameModeGoalParam(const TMap<UClass*, int>& InGoals) { Goals = InGoals; }

	UPROPERTY()
	TMap<UClass*, int> Goals;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSDFGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class USDF_API IUSDFGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PrepareGameStage() = 0;
	
	virtual void EndGameStage() = 0;
	virtual void OnPlayerDead() = 0;
	virtual void OnBossEntry(class AUSDFCharacterBossMonster* BossMonster) = 0;
	virtual void OnBossDead(class AUSDFCharacterBossMonster* BossMonster) = 0;
	virtual void OnGameRetry() = 0;
	virtual FOnGoalComplete& OnGoalCompleteDelegate() = 0;
	virtual void SetGameStage(class AUSDFGameStage* NewGameStage) = 0;
	virtual void UpdateCurrent(UClass* Class) = 0;


	virtual void SetGameModeGoal(FGameModeGoalParam&& NewGoal) = 0;
	virtual bool IsCompleteGoal() = 0;
};
