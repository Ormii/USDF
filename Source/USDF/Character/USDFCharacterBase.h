// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStat/USDFCharacterStatComponent.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "USDFCharacterBase.generated.h"

UCLASS()
class USDF_API AUSDFCharacterBase : public ACharacter, public IUSDFCharacterAttackAnimInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSDFCharacterBase();

protected:
	virtual void PostInitializeComponents() override;

	// CharacterControl Section
protected:
	virtual void SetCharacterControlData(const class UUSDFCharacterControlData* NewCharacterControlData);

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFCharacterStatComponent> Stat;

	// Attack Hit Section
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void AttackHitCheck() override;
	UPROPERTY(VisibleAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<AUSDFCharacterBase>> HitCharaters;

	// Dead Section
protected:
	virtual void SetDead();
};
