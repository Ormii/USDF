// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "USDFRangeMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFRangeMonsterAnimInstance : public UUSDFNonPlayerAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = "true"))
	uint8 bAttacking : 1;
};
