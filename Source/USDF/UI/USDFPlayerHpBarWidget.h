// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFPlayerHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFPlayerHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UUSDFPlayerHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float NewCurrentHp);
	void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
