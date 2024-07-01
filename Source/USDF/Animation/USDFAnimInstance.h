// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "USDFAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UUSDFAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector DesiredVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float SlideInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float ForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8	bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsWalk : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRun : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float TurnDotProductValue;


	FVector2D MovementInputValue;

protected:
	FVector CalculateDesiredVelocity();
	float CalculateTurnDotProductValue();


// Character Animation Data Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimationData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFCharacterAnimData> CharacterAnimData;
};
