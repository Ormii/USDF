// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "Interface/USDFCharacterAnimInterface.h"
#include "Interface/USDFDamageableInterface.h"
#include "Interface/USDFCharacterWidgetInterface.h"
#include "USDFCharacterBossMonster.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterBossMonster : public AUSDFCharacterNonPlayer, public IUSDFCharacterAnimInterface, public IUSDFDamageableInterface, public IUSDFCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	AUSDFCharacterBossMonster();

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay()override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFBossMonsterStatComponent> Stat;


	// Animation Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Intro, Meta = (AllowPrivateAccess = "true"))
	uint8 bIntroStart : 1;

	FORCEINLINE void SetIntroStart(bool NewIntroStart) { bIntroStart = NewIntroStart; }
	FORCEINLINE bool GetIntroStart() { return bIntroStart; }

	virtual float GetMaxWalkSpeed() override;
	virtual float GetMaxRunSpeed() override;
	virtual void SetLocomotionState(ELocomotionState NewLocomotionState) override;

	// AI Section
public:

	virtual float GetAIAttackRange() override;
	virtual float GetAITurnRateSpeed() override;
	virtual float GetAIEQSDefendRadius() override;


	// Damage Section
protected:
	UPROPERTY()
	TObjectPtr<class UAnimMontage> DeadAnimMontage;

	UFUNCTION()
	void OnHitReactMontageBlendOut(UAnimMontage* TargetMontage, bool bInterrupted);

	virtual float GetCurrentHealth() override;
	virtual float GetMaxHealth() override;
	virtual void Heal(float HealAmount) override;
	virtual void TakeDamage(FDamageInfo DamageInfo) override;
	virtual bool IsDead() override;

	virtual void OnDeath() override;
	virtual void OnDamageResponse(FDamageInfo DamageInfo) override;

	// Widget Section
public:
	virtual void SetupHpBarWidget(class UUSDFUserWidget* InUserWidget) override;

};
