// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNormalMonster.h"
#include "USDFCharacterRangeMonster.generated.h"

UENUM()
enum class ERangeMonsterAttackType : uint8
{
	None,
	WeakAttack,
	StrongAttack
};


/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterRangeMonster : public AUSDFCharacterNormalMonster
{
	GENERATED_BODY()

public:
	AUSDFCharacterRangeMonster();
	
protected:
	virtual void PostInitializeComponents() override;

	// AI Section
protected:
	virtual void AttackByAI(EAIAttackType InAIAttackType) override;
	virtual void AttackFinished();

	// Animation Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TMap<ERangeMonsterAttackType, TObjectPtr<class UAnimMontage>> AttackMontages;

	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void AttackFire() override;

	// Combat Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	ERangeMonsterAttackType CurrentAttackType;

	// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

	// Damage Section
protected:
	virtual void OnDeath() override;
};
