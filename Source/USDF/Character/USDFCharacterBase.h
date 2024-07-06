// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "USDFCharacterBase.generated.h"

UCLASS()
class USDF_API AUSDFCharacterBase : public ACharacter, public IUSDFCharacterAttackAnimInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSDFCharacterBase();

	// CharacterControl Section
protected:
	virtual void SetCharacterControlData(const class UUSDFCharacterControlData* NewCharacterControlData);

	// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	UPROPERTY(VisibleAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<AUSDFCharacterBase>> HitCharaters;

	// Dead Section
protected:
	virtual void SetDead();
};
