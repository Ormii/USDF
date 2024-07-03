// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/USDFCharacterControlData.h"
#include "CharacterStat/USDFCharacterStatComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AUSDFCharacterBase::AUSDFCharacterBase()
{
	RootComponent = GetCapsuleComponent();

	Stat = CreateDefaultSubobject<UUSDFCharacterStatComponent>(TEXT("Stat"));

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

void AUSDFCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->OnHpZeroDelegate.AddUObject(this, &AUSDFCharacterBase::SetDead);
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

float AUSDFCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AUSDFCharacterBase::AttackHitCheck()
{

}

void AUSDFCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
}
