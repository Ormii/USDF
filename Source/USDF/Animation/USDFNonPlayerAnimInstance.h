// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/USDFAnimInstance.h"
#include "USDFNonPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFNonPlayerAnimInstance : public UUSDFAnimInstance
{
	GENERATED_BODY()
	
public:
	UUSDFNonPlayerAnimInstance();


protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	FORCEINLINE const ELocomotionState& GetLocomotionState() const { return LocomotionState; }
	FORCEINLINE void SetLocomotionState(ELocomotionState NewLocomotionState) { LocomotionState = NewLocomotionState; }
};
