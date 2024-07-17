// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "USDFCharacterAnimData.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"
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

	if (CharacterAnimData && Movement && Owner)
	{
		/*
			Velocity = Owner->GetVelocity();
		*/
		
		IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
		if (PlayerAnimInterface)
		{
			MovementInputValue = PlayerAnimInterface->GetMovementInputValue();
			bAttackState = PlayerAnimInterface->IsAttackState();
			bIsDead = PlayerAnimInterface->IsDeadState();

			FindLocomotionState();

			
			DesiredVelocity = CalculateDesiredVelocity();
			TurnDotProductValue = CalculateTurnDotProductValue();

			/*
				ForwardInput = FMath::Lerp(ForwardInput, DesiredVelocity.Dot(Owner->GetActorForwardVector()) * (LocomotionState == ELocomotionState::Run ? 2 : 1), 0.1f);
				SlideInput = FMath::Lerp(SlideInput, DesiredVelocity.Dot(Owner->GetActorRightVector()) * (LocomotionState == ELocomotionState::Run ? 2 : 1), 0.1f);
			*/

			

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

FVector UUSDFPlayerAnimInstance::CalculateDesiredVelocity()
{
	if (Owner)
	{
		FRotator OwnerControlRotation = Owner->GetControlRotation();

		IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
		if (PlayerAnimInterface)
		{
			FRotator ControlYawRotation = FRotator(0.0f, OwnerControlRotation.Yaw, 0.0f);

			FVector ControlForwardVector = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X) * MovementInputValue.X;
			FVector ControlRightVector = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y) * MovementInputValue.Y;

			return (ControlForwardVector + ControlRightVector).GetSafeNormal();
		}
	}
	return FVector::ZeroVector;
}

float UUSDFPlayerAnimInstance::CalculateTurnDotProductValue()
{
	if (Owner)
	{
		FRotator OwnerControlRotation = Owner->GetControlRotation();

		IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
		if (PlayerAnimInterface)
		{
			FRotator ControlYawRotation = FRotator(0.0f, OwnerControlRotation.Yaw, 0.0f);

			FVector ControlForwardVector = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
			FVector ControlRightVector = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

			return (ControlForwardVector + ControlRightVector).GetSafeNormal().Dot(Owner->GetActorForwardVector());
		}
	}
	return 0.0f;
}

void UUSDFPlayerAnimInstance::FindLocomotionState()
{
	IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
	if (PlayerAnimInterface)
	{
		if (Movement->IsFalling() && (Velocity.Z > CharacterAnimData->JumpingThreshould))
			LocomotionState = ELocomotionState::Jumping;
		else if (Movement->IsFalling())
			LocomotionState = ELocomotionState::Falling;
		else if (MovementInputValue.Length() > 0.1f)
		{
			if (PlayerAnimInterface->IsSprintState() == false)
				LocomotionState = ELocomotionState::Walk;
			else if (PlayerAnimInterface->IsSprintState() == true)
				LocomotionState = ELocomotionState::Run;
		}
		else
		{
			LocomotionState = ELocomotionState::Idle;
		}
	}
}
