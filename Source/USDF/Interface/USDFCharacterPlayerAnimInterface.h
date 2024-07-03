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
	virtual void SetCombatState(bool NewCombatState) = 0;

	virtual bool IsSprintState() = 0;
	virtual bool IsCombatState() = 0;
	virtual bool IsAttackState() = 0;
	virtual FVector2D GetMovementInputValue() = 0;

	virtual void EquipWeapon() = 0;
	virtual void UnEquipWeapon() = 0;

	virtual bool CheckCombo() = 0;
};
