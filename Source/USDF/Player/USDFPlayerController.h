// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "USDFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUSDFPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnPrepareGameStageCpp"))
	void K2_OnPrepareGameStage();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnEndGameStageCpp"))
	void K2_OnEndGameStage();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnAllGameStageClearCpp"))
	void K2_OnAllGameStageClear();

	void SetTargetBoss(class AUSDFCharacterBossMonster* BossMonster);
	void OnBossDead(class AUSDFCharacterBossMonster* BossMonster);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD, Meta = (AllowPrivateAccess= "true"))
	TSubclassOf<class UUSDFPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD, Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFPlayerHUDWidget> HUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boss, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class AUSDFCharacterBossMonster> TargetBoss;
};