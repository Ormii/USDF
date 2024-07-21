// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBossDarkMage.h"
#include "AI/USDFBossDarkMageAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "CharacterStat/USDFBossMonsterStatComponent.h"

AUSDFCharacterBossDarkMage::AUSDFCharacterBossDarkMage()
{
	//Transform
	SetActorScale3D(FVector(1.7f, 1.7f, 1.7f));

	//AI
	AIControllerClass = AUSDFBossDarkMageAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 85.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 380.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/SkeletonMage/Mesh/SK_SkeletonMage.SK_SkeletonMage"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/NonPlayer/ABP_USDFBossDarkMageAnimInstance.ABP_USDFBossDarkMageAnimInstance_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}
}

void AUSDFCharacterBossDarkMage::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUSDFGameSingleton* GameSingleton = Cast<UUSDFGameSingleton>(GEngine->GameSingleton.Get());

	Stat->InitBossMonsterStat(GameSingleton->GetBossMonsterStat("DarkMage"));
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetBossMonsterStat().RunSpeed;

}

void AUSDFCharacterBossDarkMage::AttackByAI(EAIAttackType InAIAttackType)
{
}

void AUSDFCharacterBossDarkMage::AttackFinished()
{
}

void AUSDFCharacterBossDarkMage::AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
}

void AUSDFCharacterBossDarkMage::AttackFire()
{
}

void AUSDFCharacterBossDarkMage::OnDeath()
{
}
