// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Physics/USDFCollision.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "Animation/USDFLocomotionState.h"

AUSDFCharacterNonPlayer::AUSDFCharacterNonPlayer()
{
	// Capsule
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);

	// Mesh
	GetMesh()->SetCollisionProfileName(CPROFILE_USDF_NONPlAYER_CHARACTERMESH);
}

void AUSDFCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AUSDFCharacterNonPlayer::AttackFinished()
{
	OnAttackFinished.ExecuteIfBound();
}

void AUSDFCharacterNonPlayer::SetLocomotionState(ELocomotionState NewLocomotionState)
{
	UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (NonPlayerAnimInstance)
		NonPlayerAnimInstance->SetLocomotionState(NewLocomotionState);
}
