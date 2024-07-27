// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "Damage/USDFDamageCommon.h"
#include "USDFCharacterBase.generated.h"

UCLASS()
class USDF_API AUSDFCharacterBase : public ACharacter, public IUSDFCharacterAttackAnimInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSDFCharacterBase();

	// Attack Section
protected:
	UPROPERTY(VisibleAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<AUSDFCharacterBase>> HitCharaters;


	// Damage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFDamageSystemComponent> DamageSystem;

	virtual void OnDamageResponse(FDamageInfo DamageInfo) {};
	virtual void OnDeath();
};
