// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFNonPlayerAnimInstance.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "AIController.h"

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

	AUSDFCharacterNonPlayer* NonPlayer = Cast<AUSDFCharacterNonPlayer>(GetOwningActor());
	if (NonPlayer)
	{
		AAIController* AIController = Cast<AAIController>(NonPlayer->GetController());
		if (AIController)
		{
			bIsFocusingTarget = IsValid(AIController->GetFocusActor());
		}
	}
}