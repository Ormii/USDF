// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBossMonster.h"
#include "USDFCharacterBossDarkMage.generated.h"

UENUM()
enum class EDarkMageAttackType : uint8
{
	None,
	DefaultAttack,
	Attack1,
	Attack2,
	Attack3,
};


/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterBossDarkMage : public AUSDFCharacterBossMonster
{
	GENERATED_BODY()

public:
	AUSDFCharacterBossDarkMage();

protected:
	virtual void PostInitializeComponents() override;

	// AI Section
protected:
	virtual void AttackByAI(EAIAttackType InAIAttackType) override;
	virtual void AttackFinished();

	// Animation Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TMap<EDarkMageAttackType, TObjectPtr<class UAnimMontage>> AttackMontages;

	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void AttackFire() override;

	// Combat Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	EDarkMageAttackType CurrentAttackType;

	// Damage Section
protected:
	virtual void OnDeath() override;
};
