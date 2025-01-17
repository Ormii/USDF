// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USDFCharacterPlayerAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSDFCharacterPlayerAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class USDF_API IUSDFCharacterPlayerAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsCombatState() { return false; };
	virtual bool IsDeadState() = 0;

	virtual void EquipWeapon() {};
	virtual void UnEquipWeapon() {};

	virtual void CheckCombo() {};

	virtual void CheckApplyDamagePoint() {}
	virtual class UParticleSystem* GetTrailParticleSystem() { return nullptr; }
	virtual FName GetTrailStartSocketName() { return ""; }
	virtual FName GetTrailEndSocketName() { return ""; }

};
