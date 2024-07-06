// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFNonPlayerAnimInstance.h"

UUSDFNonPlayerAnimInstance::UUSDFNonPlayerAnimInstance()
{

}

void UUSDFNonPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UUSDFNonPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	GroundSpeed = Velocity.Size2D();
}