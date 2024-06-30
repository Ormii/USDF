// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "USDFCharacterAnimData.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"

UUSDFAnimInstance::UUSDFAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UUSDFCharacterAnimData> CharacterAnimDataRef(TEXT("/Game/Animation/DA_USDF_AnimData.DA_USDF_AnimData"));
	if (CharacterAnimDataRef.Object)
	{
		CharacterAnimData = CharacterAnimDataRef.Object;
	}
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

	if (CharacterAnimData)
	{
		if (Movement)
		{
			Velocity = Movement->Velocity;
			GroundSpeed = Velocity.Size2D();
			bIsFalling = Movement->IsFalling();

			bIsIdle = GroundSpeed < CharacterAnimData->MovingThreshould;
			bIsJumping = Movement->IsFalling() && (Velocity.Z > CharacterAnimData->JumpingThreshould);
		}

		if (Owner)
		{
			FRotator WorldRotation = Owner->GetActorRotation(); 
			Direction = CalculateDirection(Velocity, WorldRotation);

			IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(Owner);
			if (PlayerAnimInterface)
			{
				PreGroundSpeed = PlayerAnimInterface->GetPreGroundSpeed();
				PreVelocity = PlayerAnimInterface->GetPreVelocity();
				AddVelocityScale = PlayerAnimInterface->GetAddVelocityScale();
				bIsWalk = (PlayerAnimInterface->IsSprintState() == false && !bIsIdle);
				bIsRun = (PlayerAnimInterface->IsSprintState() == true && !bIsIdle);

				PreDirection = CalculateDirection(PreVelocity, WorldRotation);
			}
		}
	}
}
