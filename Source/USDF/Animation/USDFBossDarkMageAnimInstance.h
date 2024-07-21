// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "USDFBossDarkMageAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBossDarkMageAnimInstance : public UUSDFNonPlayerAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float SpeedSide;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float SpeedForward;
};
