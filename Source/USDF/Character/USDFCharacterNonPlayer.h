// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBase.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "USDFCharacterNonPlayer.generated.h"

/*

Passive UMETA(DisplayName = "Passive"),
Attacking UMETA(DisplayName = "Attacking"),
Frozen UMETA(DisplayName = "Frozen"),
Investigating UMETA(DisplayName = "Investigating"),
Dead UMETA(DisplayName = "Dead"),
*/
USTRUCT()
struct FAIStateChangeWrapper
{
	GENERATED_BODY()

	FAIStateChangeWrapper() {};
	FAIStateChangeWrapper(FOnChangeAIState InOnChangeAIState) { OnChangeAIState = InOnChangeAIState; }
	FOnChangeAIState OnChangeAIState;
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterNonPlayer : public AUSDFCharacterBase, public IUSDFCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AUSDFCharacterNonPlayer();

	// AI Section
protected:
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)override;

	FAICharacterAttackFinished OnAttackFinished;
	virtual void AttackFinished();
	virtual void SetLocomotionState(ELocomotionState NewLocomotionState) override;
	virtual void SetAIState(EAIState NewAIState, FAISensedParam InParam) override;

	virtual void SetAIStatePassive(FAISensedParam InParam);
	virtual void SetAIStateAttacking(FAISensedParam InParam);
	virtual void SetAIStateFrozen(FAISensedParam InParam);
	virtual void SetAIStateInvestigating(FAISensedParam InParam);
	virtual void SetAIStateDead(FAISensedParam InParam);

protected:
	UPROPERTY(VisibleAnywhere, Category = AIState, Meta =(AllowPrivateAccess= "true"))
	EAIState CurrentAIState;

	UPROPERTY(VisibleAnywhere, Category = AIState, Meta = (AllowPrivateAccess = "true"))
	TMap<EAIState, FAIStateChangeWrapper> AIStateManager;

	UPROPERTY(VisibleAnywhere, Category = AIState, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<AActor> Target;
};
