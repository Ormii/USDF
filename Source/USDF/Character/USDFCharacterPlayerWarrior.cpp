// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterPlayerWarrior.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AUSDFCharacterPlayerWarrior::AUSDFCharacterPlayerWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/IdaFaber/Meshes/Girl/SK_CALISTA_01.SK_CALISTA_01"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/Player/ABP_USDFPlayerWarrior.ABP_USDFPlayerWarrior_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CombatStartMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_CombatStart.AM_USDF_PlayerWarrior_CombatStart"));
	if (CombatStartMontageRef.Object)
	{
		CombatStartMontage = CombatStartMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CombatEndMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_CombatEnd.AM_USDF_PlayerWarrior_CombatEnd"));
	if (CombatEndMontageRef.Object)
	{
		CombatEndMontage = CombatEndMontageRef.Object;
	}
}

void AUSDFCharacterPlayerWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void AUSDFCharacterPlayerWarrior::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CombatStateTime > 0 && bCombatState)
	{
		CombatStateTime -= DeltaSeconds;

		UE_LOG(LogTemp, Display, TEXT("CombatStateTime : %f"), CombatStateTime);
		if (CombatStateTime <= 0)
		{
			CombatStateTime = 0;
			UE_LOG(LogTemp, Display, TEXT("CombateEndMontageStart"));
			PossessCombatEndMontage();
		}
	}
}

void AUSDFCharacterPlayerWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::Attack);
}

void AUSDFCharacterPlayerWarrior::Attack()
{
	Super::Attack();

	UE_LOG(LogTemp, Display, TEXT("AttackStart"));
	
	PossessCombatStartMontage();
}

void AUSDFCharacterPlayerWarrior::PossessCombatStartMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (bCombatState == false)
	{
		if (AnimInstance->Montage_IsPlaying(CombatStartMontage) == true)
			return;
		
		AnimInstance->Montage_Play(CombatStartMontage, 1.0f);
	
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::CombatStartMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, CombatStartMontage);
		return;
	}

	if (AnimInstance->Montage_IsPlaying(CombatEndMontage) == true)
		AnimInstance->Montage_Stop(0.2f, CombatEndMontage);

	CombatStateTime = 10;
}

void AUSDFCharacterPlayerWarrior::PossessCombatEndMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CombatEndMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::CombatEndMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CombatEndMontage);
}

void AUSDFCharacterPlayerWarrior::CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("CombatStartMontageEnd Succeed"));
	SetCombatState(true);
}

void AUSDFCharacterPlayerWarrior::CombatEndMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	SetCombatState(false);
}

void AUSDFCharacterPlayerWarrior::SetCombatState(bool NewCombatState)
{
	Super::SetCombatState(NewCombatState);
	
	UE_LOG(LogTemp, Display, TEXT("Combat State : %d"), bCombatState);

	if(NewCombatState == true)
		CombatStateTime = 10;

}