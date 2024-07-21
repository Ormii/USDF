// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"
#include "USDFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFGameMode : public AGameModeBase, public IUSDFGameModeInterface
{
	GENERATED_BODY()
	
public:
	AUSDFGameMode();

	// GameMode Section
public:
	UFUNCTION(BlueprintCallable)
	virtual void PrepareGameStage() override;

	virtual void EndGameStage() override;
	virtual void OnPlayerDead() override;
	virtual void OnBossEntry(class AUSDFCharacterBossMonster* BossMonster) override;
	virtual void OnBossDead(class AUSDFCharacterBossMonster* BossMonster) override;
	virtual FOnGoalComplete& OnGoalCompleteDelegate() { return OnGoalComplete; };
	virtual void SetGameStage(class AUSDFGameStage* NewGameStage);
	virtual bool IsCompleteGoal() override;
	virtual void UpdateCurrent(UClass* Class) override;


	UFUNCTION(BlueprintCallable)
	void BattleGameStage();

	virtual void OnGameRetry() override;

	void IncreaseCurrentGoalValue(UClass* Class);

	// Game Goal Section
protected:
	UPROPERTY(VisibleAnywhere, Category = GameGoal, Meta = (AllowPrivateAccess = "true"))
	FGameModeGoalParam Target;

	UPROPERTY(VisibleAnywhere, Category = GameGoal, Meta = (AllowPrivateAccess= "true"))
	FGameModeGoalParam Current;

	virtual void SetGameModeGoal(FGameModeGoalParam&& NewGoal)override;

public:
	FOnGoalComplete OnGoalComplete;
	TObjectPtr<class AUSDFGameStage> GameStage;
};
