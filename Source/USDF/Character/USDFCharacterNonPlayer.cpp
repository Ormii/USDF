// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Physics/USDFCollision.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "Animation/USDFLocomotionState.h"
#include "GameFramework/CharacterMovementComponent.h"

AUSDFCharacterNonPlayer::AUSDFCharacterNonPlayer()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Character
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Capsule
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);

	// Mesh
	GetMesh()->SetCollisionProfileName("NoCollision");
}

void AUSDFCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
	AIStateManager.Add(EAIState::Passive, FOnChangeAIState::CreateUObject(this, &AUSDFCharacterNonPlayer::SetAIStatePassive));
	AIStateManager.Add(EAIState::Attacking, FOnChangeAIState::CreateUObject(this, &AUSDFCharacterNonPlayer::SetAIStateAttacking));
	AIStateManager.Add(EAIState::Frozen, FOnChangeAIState::CreateUObject(this, &AUSDFCharacterNonPlayer::SetAIStateFrozen));
	AIStateManager.Add(EAIState::Investigating, FOnChangeAIState::CreateUObject(this, &AUSDFCharacterNonPlayer::SetAIStateInvestigating));
	AIStateManager.Add(EAIState::Dead, FOnChangeAIState::CreateUObject(this, &AUSDFCharacterNonPlayer::SetAIStateDead));
}

void AUSDFCharacterNonPlayer::SetAIActionDelegate(const FAICharacterActionFinished& InOnActionFinished)
{
	OnActionFinished = InOnActionFinished;
}

void AUSDFCharacterNonPlayer::ActionFinished()
{
	OnActionFinished.ExecuteIfBound();
}

void AUSDFCharacterNonPlayer::SetLocomotionState(ELocomotionState NewLocomotionState)
{
	UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (NonPlayerAnimInstance)
		NonPlayerAnimInstance->SetLocomotionState(NewLocomotionState);
}

void AUSDFCharacterNonPlayer::SetAIState(EAIState NewAIState, FAISensedParam InParam)
{
	CurrentAIState = NewAIState;
	if (AIStateManager.Find(CurrentAIState) != nullptr)
	{
		AIStateManager[CurrentAIState].OnChangeAIState.ExecuteIfBound(InParam);
	}
}

void AUSDFCharacterNonPlayer::SetAIStatePassive(FAISensedParam InParam)
{
	
}

void AUSDFCharacterNonPlayer::SetAIStateAttacking(FAISensedParam InParam)
{
	
}

void AUSDFCharacterNonPlayer::SetAIStateFrozen(FAISensedParam InParam)
{
	
}

void AUSDFCharacterNonPlayer::SetAIStateInvestigating(FAISensedParam InParam)
{
	
}

void AUSDFCharacterNonPlayer::SetAIStateDead(FAISensedParam InParam)
{
	
}
