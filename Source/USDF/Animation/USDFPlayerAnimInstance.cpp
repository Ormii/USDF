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
		Velocity = Owner->GetVelocity();
		
		IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
		if (PlayerAnimInterface)
		{
			MovementInputValue = PlayerAnimInterface->GetMovementInputValue();
			bAttackState = PlayerAnimInterface->IsAttackState();
			bIsWalk = (PlayerAnimInterface->IsSprintState() == false && !bIsIdle);
			bIsRun = (PlayerAnimInterface->IsSprintState() == true && !bIsIdle);
			bIsDead = PlayerAnimInterface->IsDeadState();

			DesiredVelocity = CalculateDesiredVelocity();
			TurnDotProductValue = CalculateTurnDotProductValue();
			
			ForwardInput = FMath::Lerp(ForwardInput, DesiredVelocity.Dot(Owner->GetActorForwardVector()) * (bIsRun ? 2 : 1), 0.1f);
			SlideInput = FMath::Lerp(SlideInput, DesiredVelocity.Dot(Owner->GetActorRightVector()) * (bIsRun ? 2 : 1), 0.1f);

			if (!bIsIdle)
			{
				FRotator OwnerRotation = Owner->GetActorRotation();
				FRotator OwnerControlRotation = Owner->GetControlRotation();

				FRotator NewOwnerRotation = FMath::RInterpTo(OwnerRotation, OwnerControlRotation, GetWorld()->GetDeltaSeconds(), 1.0f);
				Owner->SetActorRotation(FRotator(0.0f, NewOwnerRotation.Yaw, 0.0f));
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
