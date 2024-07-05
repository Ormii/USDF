// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/USDFAnimInstance.h"
#include "USDFPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFPlayerAnimInstance : public UUSDFAnimInstance
{
	GENERATED_BODY()

public:
	UUSDFPlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsWalk : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRun : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector DesiredVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float SlideInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float TurnDotProductValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bAttackState : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;

	FVector2D MovementInputValue;
protected:
	FVector CalculateDesiredVelocity();
	float CalculateTurnDotProductValue();
};
