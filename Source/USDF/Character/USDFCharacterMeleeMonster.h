// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNormalMonster.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "USDFCharacterMeleeMonster.generated.h"

DECLARE_DELEGATE(FOnMeleeMonsterAttackCheckDelegate)

UENUM()
enum class EMeleeMonsterActionType : uint8
{
	None,
	WeakAttack,
	StrongAttack
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterMeleeMonster : public AUSDFCharacterNormalMonster
{
	GENERATED_BODY()

public:
	AUSDFCharacterMeleeMonster();

protected:
	virtual void PostInitializeComponents() override;

	// AI Section
protected:
	virtual void ActionByAI(EAIActionType InAIActionType) override;
	virtual void ActionFinished() override;

	// Animation Section
public:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess= "true"))
	TMap<EMeleeMonsterActionType, TObjectPtr<class UAnimMontage>> ActionMontages;

	virtual void AttackHitCheck() override;

private:
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// Combat Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess= "true"))
	EMeleeMonsterActionType CurrentActionType;

	void WeakAttackHitCheck();
	void StrongAttackHitCheck();

	// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

	// Damage Section
protected:
	virtual void OnDeath() override;

};
