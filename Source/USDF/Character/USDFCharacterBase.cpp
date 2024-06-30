// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/USDFCharacterControlData.h"

// Sets default values
AUSDFCharacterBase::AUSDFCharacterBase()
{
	RootComponent = GetCapsuleComponent();

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/ReferenceAsset/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

}

void AUSDFCharacterBase::SetCharacterControlData(const UUSDFCharacterControlData* NewCharacterControlData)
{
	//Pawn
	bUseControllerRotationYaw = NewCharacterControlData->bUseControllerRotationYaw;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = NewCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->RotationRate = NewCharacterControlData->RotationRate;
	GetCharacterMovement()->bUseControllerDesiredRotation = NewCharacterControlData->bUseControllerDesiredRotation;
}
