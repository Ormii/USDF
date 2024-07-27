// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBase.h"
#include "Interface/USDFCharacterAIInterface.h"
#include "USDFCharacterNonPlayer.generated.h"

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


protected:
	void BeginPlay();

	// AI Section
protected:
	UPROPERTY(VisibleAnywhere, Category = AIState, Meta = (AllowPrivateAccess = "true"))
	EAIState CurrentAIState;

	UPROPERTY(VisibleAnywhere, Category = AIState, Meta = (AllowPrivateAccess = "true"))
	TMap<EAIState, FAIStateChangeWrapper> AIStateManager;

	FAICharacterActionFinished OnActionFinished;

	virtual void SetAIActionDelegate(const FAICharacterActionFinished& InOnActionFinished)override;
	virtual void ActionFinished();
	virtual void SetLocomotionState(ELocomotionState NewLocomotionState) override;
	virtual void SetAIState(EAIState NewAIState, FAISensedParam InParam) override;

	virtual void SetAIStatePassive(FAISensedParam InParam);
	virtual void SetAIStateAttacking(FAISensedParam InParam);
	virtual void SetAIStateFrozen(FAISensedParam InParam);
	virtual void SetAIStateInvestigating(FAISensedParam InParam);
	virtual void SetAIStateDead(FAISensedParam InParam);
};
