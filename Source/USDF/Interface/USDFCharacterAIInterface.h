// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Animation/USDFLocomotionState.h"
#include "AI/USDFAICommon.h"
#include "USDFCharacterAIInterface.generated.h"

DECLARE_DELEGATE(FAICharacterActionFinished);

UENUM(BlueprintType)
enum class EAIActionType : uint8
{
	None,
	Melee,
	Range,
	Dash,
	Attack1,
	Attack2,
	Attack3,
	Attack4,
	Attack5,
	Attack6,

	Buff1,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSDFCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class USDF_API IUSDFCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() { return 0.0f; }
	virtual float GetAIDetectRadius() { return 0.0f; }
	virtual float GetAIAttackRange() { return 0.0f; }
	virtual float GetAITurnRateSpeed() { return 0.0f; }
	virtual class AUSDFPatrolRoute* GetPatrolRoute() { return nullptr; }
	virtual float GetAIEQSTargetRadius(){ return 0.0f; }
	virtual float GetAIEQSDefendRadius() { return 0.0f; }

	virtual void SetLocomotionState(ELocomotionState NewLocomotionState) = 0;
	virtual void SetAIState(EAIState NewAIState, FAISensedParam InParam) = 0;

	virtual void SetAIActionDelegate(const FAICharacterActionFinished& InOnAttackFinished){};
	virtual void ActionByAI(EAIActionType InAIAttackType) {}
};
