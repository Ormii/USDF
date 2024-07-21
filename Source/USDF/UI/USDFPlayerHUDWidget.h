// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUSDFPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	class UUSDFEnemyHpBarWidget* GetBossHpBar() { return BossHpBar; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFPlayerHpBarWidget> PlayerHpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameStage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFPrepareStageWidget> PrepareStageWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameStage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFEndStageWidget> EndStageWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Boss, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFEnemyHpBarWidget> BossHpBar;
};
