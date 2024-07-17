// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "Interface/USDFCharacterWidgetInterface.h"
#include "Interface/USDFCharacterHitReactInterface.h"
#include "Interface/USDFCharacterAnimInterface.h"
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
	public IUSDFCharacterHitReactInterface, public IUSDFCharacterAnimInterface
{
	GENERATED_BODY()

public:
	AUSDFCharacterNormalMonster();

protected:
	virtual void PostInitializeComponents() override;

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = AI, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class AUSDFPatrolRoute> PatrolRoute;

	// Hit React Section
protected:
	UPROPERTY()
	TMap<EHitReactDirection, TObjectPtr<class UAnimMontage>> HitReactAnimMontage;

	UPROPERTY()
	uint8 bHitReactState : 1;

	UPROPERTY()
	EHitReactType CurrentHitReactType;

	float HitReactTime;
	float UpperHitTime;
	
	virtual bool GetHitReactState() override;
public:
	virtual void HitReact(const float DamageAmount, EHitReactType HitReactType, const AActor* HitCauser) override;

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

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFNormalMonsterStatComponent> Stat;

	// Attack Hit Section
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Animation Section
public:
	virtual float GetMaxWalkSpeed() override;
	virtual float GetMaxRunSpeed() override;
	virtual void SetLocomotionState(ELocomotionState NewLocomotionState) override;
};
