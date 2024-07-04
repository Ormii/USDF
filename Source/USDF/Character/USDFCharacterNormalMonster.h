// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "Interface/USDFCharacterWidgetInterface.h"
#include "Interface/USDFCharacterHitReactInterface.h"
#include "USDFCharacterNormalMonster.generated.h"

UENUM()
enum class EHitReactDirection : uint8
{
	Front,
	Back,
	Right,
	Left
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterNormalMonster : public AUSDFCharacterNonPlayer, public IUSDFCharacterWidgetInterface, 
	public IUSDFCharacterHitReactInterface
{
	GENERATED_BODY()

public:
	AUSDFCharacterNormalMonster();

public:
	virtual void Tick(float DeltaSeconds) override;

	// Hit React Section
protected:
	UPROPERTY()
	TMap<EHitReactDirection, TObjectPtr<class UAnimMontage>> HitReactAnimMontage;

	UPROPERTY()
	uint8 bHitReactState : 1;
	float HitReactTime;

	virtual bool GetHitReactState() override;
public:
	virtual void HitReact(const FHitResult& HitResult, const float DamageAmount, const AActor* HitCauser) override;

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
