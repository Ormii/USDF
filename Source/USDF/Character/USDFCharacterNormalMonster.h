// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "Interface/USDFCharacterWidgetInterface.h"
#include "Interface/USDFCharacterAnimInterface.h"
#include "Interface/USDFDamageableInterface.h"
#include "USDFCharacterNormalMonster.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterNormalMonster : public AUSDFCharacterNonPlayer, public IUSDFCharacterWidgetInterface,
	public IUSDFCharacterAnimInterface, public IUSDFDamageableInterface
{
	GENERATED_BODY()

public:
	AUSDFCharacterNormalMonster();

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRadius() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnRateSpeed() override;
	virtual float GetAIEQSTargetRadius() override;
	virtual float GetAIEQSDefendRadius() override;
	virtual class AUSDFPatrolRoute* GetPatrolRoute() override;
	virtual float GetMaxHealth() override;
	virtual float GetCurrentHealth() override;
	virtual void Heal(float HealAmount) override;
	virtual void TakeDamage(FDamageInfo DamageInfo) override;
	virtual bool IsDead() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = AI, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class AUSDFPatrolRoute> PatrolRoute;

	// Hit React Section
protected:
	UPROPERTY()
	TMap<EHitReactDirection, TObjectPtr<class UAnimMontage>> HitReactAnimMontage;
	
	// Widget Section
public:
	virtual void SetupHpBarWidget(class UUSDFUserWidget* InUserWidget) override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFWidgetComponent> HpBarWidget;

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFNormalMonsterStatComponent> Stat;

	// Animation Section
public:
	virtual float GetMaxWalkSpeed() override;
	virtual float GetMaxRunSpeed() override;
	virtual void SetLocomotionState(ELocomotionState NewLocomotionState) override;

	// Damage Section
protected:
	UPROPERTY()
	TObjectPtr<class UAnimMontage> DeadAnimMontage;

	UFUNCTION()
	void OnHitReactMontageBlendOut(UAnimMontage* TargetMontage, bool bInterrupted);

	virtual void OnDeath() override;
	virtual void OnDamageResponse(FDamageInfo DamageInfo) override;
};
