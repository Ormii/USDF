// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "USDFCharacterAnimData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/USDFCharacterAnimInterface.h"

UUSDFAnimInstance::UUSDFAnimInstance()
{

}

void UUSDFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UUSDFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement && Owner)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();

		ACharacter* Character = Cast<ACharacter>(GetOwningActor());

		if (Character)
		{
			const FRotator Rotaion = Character->GetControlRotation();
			Angle = CalculateDirection(Velocity, Rotaion);
		}
	}
}