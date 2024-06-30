// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFPlayerWarriorAnimInstance.h"
#include "GameFramework/Character.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"

UUSDFPlayerWarriorAnimInstance::UUSDFPlayerWarriorAnimInstance()
{

}

void UUSDFPlayerWarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UUSDFPlayerWarriorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner)
	{
		IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
		if (PlayerAnimInterface)
		{
			bIsCombatState = PlayerAnimInterface->IsCombatState();
		}
	}
}
