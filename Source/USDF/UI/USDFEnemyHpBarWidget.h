// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/USDFUserWidget.h"
#include "USDFEnemyHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFEnemyHpBarWidget : public UUSDFUserWidget
{
	GENERATED_BODY()
	
public:
	UUSDFEnemyHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
public:
	void UpdateHpBar(float NewCurrentHp);

	void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> BackgroundHpBar;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY()
	float BackgroundHp;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
