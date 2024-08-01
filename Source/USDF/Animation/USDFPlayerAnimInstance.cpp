// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"
#include "Interface/USDFCharacterPlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"

UUSDFPlayerAnimInstance::UUSDFPlayerAnimInstance()
{

}

void UUSDFPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UUSDFPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement && Owner)
	{
		IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
		if (PlayerAnimInterface)
		{
			bIsDead = PlayerAnimInterface->IsDeadState();

			FindLocomotionState();

			if (LocomotionState != ELocomotionState::Idle)
			{
				FRotator OwnerRotation = Owner->GetActorRotation();
				FRotator OwnerControlRotation = Owner->GetControlRotation();

				FRotator NewOwnerRotation = FMath::RInterpTo(OwnerRotation, OwnerControlRotation, GetWorld()->GetDeltaSeconds(), 1.0f);
				Owner->SetActorRotation(FRotator(0.0f, NewOwnerRotation.Yaw, 0.0f));
			}
			
			ACharacter* Character = Cast<ACharacter>(GetOwningActor());
			if (Character)
			{
				const FRotator Rotaion = Character->GetActorRotation();
				Angle = CalculateDirection(Velocity, Rotaion);
			}
		}
	}
}

void UUSDFPlayerAnimInstance::FindLocomotionState()
{
	IUSDFCharacterPlayerInterface* PlayerInterface = Cast<IUSDFCharacterPlayerInterface>(Owner);
	if (PlayerInterface)
	{
		if (GroundSpeed > PlayerInterface->GetMaxWalkSpeed())
		{
			LocomotionState = ELocomotionState::Run;
		}
		else if (MovementInputValue.Length() > 0.1f)
		{
			LocomotionState = ELocomotionState::Walk;
		}
		else
		{
			LocomotionState = ELocomotionState::Idle;
		}
	}
}
