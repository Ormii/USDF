// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNormalMonster.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "USDFCharacterMeleeMonster.generated.h"

DECLARE_DELEGATE(FOnMeleeMonsterAttackCheckDelegate)

UENUM()
enum class EMeleeMonsterAttackType : uint8
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
	virtual void AttackByAI(EAIAttackType InAIAttackType) override;
	virtual void AttackFinished();

	// Hit React Section
public:
	virtual void HitReact(const float DamageAmount, EHitReactType HitReactType ,const AActor* HitCauser) override;

	// Animation Section
public:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess= "true"))
	TMap<EMeleeMonsterAttackType, TObjectPtr<class UAnimMontage>> AttackMontages;

	virtual void AttackHitCheck() override;

private:
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// Combat Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess= "true"))
	EMeleeMonsterAttackType CurrentAttackType;

	void WeakAttackHitCheck();
	void StrongAttackHitCheck();

	// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

};
