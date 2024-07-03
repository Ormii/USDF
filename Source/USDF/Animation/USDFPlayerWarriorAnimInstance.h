// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/USDFPlayerAnimInstance.h"
#include "USDFPlayerWarriorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFPlayerWarriorAnimInstance : public UUSDFPlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	UUSDFPlayerWarriorAnimInstance();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnJump"))
	void K2_OnJump();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsCombatState : 1;
};
