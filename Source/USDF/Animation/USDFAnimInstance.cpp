// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "USDFCharacterAnimData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/USDFCharacterAnimInterface.h"

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

	if (CharacterAnimData && Movement && Owner)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		Acceleration = Movement->GetCurrentAcceleration();
		Acceleration = FVector(Acceleration.X, Acceleration.Y, 0.0f);

		ACharacter* Character = Cast<ACharacter>(GetOwningActor());

		if (Character)
		{
			const FRotator Rotaion = Character->GetControlRotation();
			Angle = CalculateDirection(Velocity, Rotaion);
		}
	}
}