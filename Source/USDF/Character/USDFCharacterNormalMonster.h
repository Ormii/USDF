// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "Interface/USDFCharacterWidgetInterface.h"
#include "USDFCharacterNormalMonster.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterNormalMonster : public AUSDFCharacterNonPlayer, public IUSDFCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	AUSDFCharacterNormalMonster();

	// Dead Section
protected:
	virtual void SetDead() override;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> DeadAnimMontage;

	// Widget Section
public:
	virtual void SetupHpBarWidget(class UUSDFUserWidget* InUserWidget) override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFWidgetComponent> HpBarWidget;
};
