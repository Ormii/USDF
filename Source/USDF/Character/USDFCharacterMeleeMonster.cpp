// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterMeleeMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/USDFMeleeMonsterAnimInstance.h"
#include "Physics/USDFCollision.h"
#include "AI/USDFMeleeMonsterAIController.h"

AUSDFCharacterMeleeMonster::AUSDFCharacterMeleeMonster()
{
	//AI
	AIControllerClass = AUSDFMeleeMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 85.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 190.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_USDFCAPSULE);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/Biochemical_Monster_2/baze_mech/SK_baze_mesh.SK_baze_mesh"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/NonPlayer/ABP_USDFMeleeMonsterAnimInstance.ABP_USDFMeleeMonsterAnimInstance_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadAnimMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/AM_USDF_MeleeMonsterDeadAction.AM_USDF_MeleeMonsterDeadAction"));
	if (DeadAnimMontageRef.Object)
	{
		DeadAnimMontage = DeadAnimMontageRef.Object;
	}
}

void AUSDFCharacterMeleeMonster::SetDead()
{
	Super::SetDead();
}
