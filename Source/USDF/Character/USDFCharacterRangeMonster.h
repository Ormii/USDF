// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterNormalMonster.h"
#include "USDFCharacterRangeMonster.generated.h"

UENUM()
enum class ERangeMonsterActionType : uint8
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
	virtual void ActionByAI(EAIActionType InAIAttackType) override;
	virtual void ActionFinished()override;

	// Animation Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TMap<ERangeMonsterActionType, TObjectPtr<class UAnimMontage>> ActionMontages;

	UFUNCTION()
	void ActionMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void AttackFire() override;

	// Combat Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	ERangeMonsterActionType CurrentActionType;

	// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

	// Damage Section
protected:
	virtual void OnDeath() override;
};
