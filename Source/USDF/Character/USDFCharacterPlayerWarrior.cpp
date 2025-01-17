// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterPlayerWarrior.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Animation/USDFPlayerWarriorAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/USDFComboActionData.h"
#include "Physics/USDFCollision.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CharacterStat/USDFPlayerStatComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "USDFCharacterControlData.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/USDFLocomotionState.h"
#include "Item/USDFItemWeaponDarkSword.h"
#include "Item/USDFWeaponItemData.h"
#include "Perception/AISense_Damage.h"
#include "Damage/USDFDamageSystemComponent.h"
#include "Perception/AISense_Damage.h"

AUSDFCharacterPlayerWarrior::AUSDFCharacterPlayerWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/IdaFaber/Meshes/Girl/SK_CALISTA_01.SK_CALISTA_01"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/Player/ABP_USDF_PlayerWarrior.ABP_USDF_PlayerWarrior_C"));
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

	static ConstructorHelpers::FClassFinder<AUSDFItemWeapon> WeaponSwordClassRef(TEXT("/Game/Item/BP_USDFItemWeaponLightSword.BP_USDFItemWeaponLightSword_C"));
	if (WeaponSwordClassRef.Class)
	{
		WeaponSwordClass = WeaponSwordClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> DefaultComboAttackDataRef(TEXT("/Game/CharacterAction/USDF_ComboAttackData"));
	if (DefaultComboAttackDataRef.Object)
	{
		ComboAttackDataManager.Add(EPlayerWarriorComboType::Default, DefaultComboAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> UpperAttackDataRef(TEXT("/Game/CharacterAction/USDF_UpperAttackData.USDF_UpperAttackData"));
	if (UpperAttackDataRef.Object)
	{
		ComboAttackDataManager.Add(EPlayerWarriorComboType::UpperCut, UpperAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> DashAttackDataRef(TEXT("/Game/CharacterAction/USDF_DashAttackData.USDF_DashAttackData"));
	if (DashAttackDataRef.Object)
	{
		ComboAttackDataManager.Add(EPlayerWarriorComboType::Dash, DashAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> PowerAttackDataRef(TEXT("/Game/CharacterAction/USDF_PowerAttackData.USDF_PowerAttackData"));
	if (PowerAttackDataRef.Object)
	{
		ComboAttackDataManager.Add(EPlayerWarriorComboType::Power, PowerAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactFrontMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_HitReact_Front.AM_USDF_PlayerWarrior_HitReact_Front"));
	if (HitReactFrontMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Front, HitReactFrontMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactBackMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_HitReact_Back.AM_USDF_PlayerWarrior_HitReact_Back"));
	if (HitReactBackMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Back, HitReactBackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactRightMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_HitReact_Right.AM_USDF_PlayerWarrior_HitReact_Right"));
	if (HitReactRightMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Right, HitReactRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactLeftMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_HitReact_Left.AM_USDF_PlayerWarrior_HitReact_Left"));
	if (HitReactLeftMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Left, HitReactLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeLeftMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_Left.AM_USDF_PlayerWarrior_Dodge_Left"));
	if (DodgeLeftMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Left, DodgeLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeRightMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_Right.AM_USDF_PlayerWarrior_Dodge_Right"));
	if (DodgeRightMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Right, DodgeRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeFrontMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_Front.AM_USDF_PlayerWarrior_Dodge_Front"));
	if (DodgeFrontMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Front, DodgeFrontMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeBackMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_Back.AM_USDF_PlayerWarrior_Dodge_Back"));
	if (DodgeBackMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Back, DodgeBackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeFrontLeftMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_FrontLeft.AM_USDF_PlayerWarrior_Dodge_FrontLeft"));
	if (DodgeFrontLeftMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Front_Left, DodgeFrontLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeFrontRightMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_FrontRight.AM_USDF_PlayerWarrior_Dodge_FrontRight"));
	if (DodgeFrontRightMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Front_Right, DodgeFrontRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeBackLeftMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_BackLeft.AM_USDF_PlayerWarrior_Dodge_BackLeft"));
	if (DodgeBackLeftMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Back_Left, DodgeBackLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeBackRightMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Dodge_BackRight.AM_USDF_PlayerWarrior_Dodge_BackRight"));
	if (DodgeBackRightMontageRef.Object)
	{
		DodgeAnimMontage.Add(EPlayerDodgeDirection::Back_Right, DodgeBackRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactKnockBackMontageRef(TEXT("/Game/Animation/Player/Warrior/AM_USDF_PlayerWarrior_Knockback.AM_USDF_PlayerWarrior_Knockback"));
	if (HitReactKnockBackMontageRef.Object)
	{
		HitReactKnockBackMontage = HitReactKnockBackMontageRef.Object;
	}

	for (int i = 0; i < 3; ++i)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackHitEffectRef(*FString::Printf(TEXT("/Game/ReferenceAsset/RealisticBlood/Slash/Niagara/NS_Slash_%d.NS_Slash_%d"),i,i));
		if (AttackHitEffectRef.Object)
		{
			AttackHitEffects.Add(AttackHitEffectRef.Object);
		}
	}

	CurrentComboAttackType = EPlayerWarriorComboType::None;
	ComboAttackDelegateManager.Add(EPlayerWarriorComboType::None, FComboAttackDelegateWrapper());
	ComboAttackDataManager.Add(EPlayerWarriorComboType::None, nullptr);
}

void AUSDFCharacterPlayerWarrior::BeginPlay()
{
	Super::BeginPlay();

	WeaponSword = GetWorld()->SpawnActor<AUSDFItemWeapon>(WeaponSwordClass, FTransform::Identity);
	WeaponSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_socket");
	WeaponSword->GetMesh()->SetCollisionProfileName("NoCollision");

	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::DefaultComboAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::Default, Wrapper);
	}

	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::UpperAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::UpperCut, Wrapper);
	}

	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::DashAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::Dash, Wrapper);
	}

	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::PowerAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::Power, Wrapper);
	}

	{
		DamageSystem->OnDamageResponse.BindUObject(this, &AUSDFCharacterPlayerWarrior::OnDamageResponse);
	}
}

void AUSDFCharacterPlayerWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUSDFGameSingleton* GameSingleton = Cast<UUSDFGameSingleton>(GEngine->GameSingleton.Get());

	Stat->InitPlayerStat(GameSingleton->GetPlayerStat("PlayerWarrior"));
}

void AUSDFCharacterPlayerWarrior::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CombatStateTime > 0 && bCombatState)
	{
		CombatStateTime -= DeltaSeconds;

		if (CombatStateTime <= 0)
		{
			CombatStateTime = 0;
			PossessCombatEndMontage();
		}
	}
}

void AUSDFCharacterPlayerWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::Attack);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::ReleaseAttack);
	EnhancedInputComponent->BindAction(AttackQKeyAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::AttackQKey);
	EnhancedInputComponent->BindAction(AttackQKeyAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::ReleaseAttackQKey);
	EnhancedInputComponent->BindAction(AttackEKeyAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::AttackEKey);
	EnhancedInputComponent->BindAction(AttackEKeyAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::ReleaseAttackEKey);
	EnhancedInputComponent->BindAction(AttackRKeyAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::AttackRKey);
	EnhancedInputComponent->BindAction(AttackRKeyAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::ReleaseAttackRKey);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::Dodge);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::StopDodge);
}

void AUSDFCharacterPlayerWarrior::Attack()
{
	Super::Attack();

	if (bDamagedState)
		return;

	if (PossessCombatStartMontage())
	{
		PossessAttackMontage();
	}
}

void AUSDFCharacterPlayerWarrior::ReleaseAttack()
{
	Super::ReleaseAttack();
}

void AUSDFCharacterPlayerWarrior::AttackQKey()
{
	Super::AttackQKey();

	if (bDamagedState)
		return;

	if (bCombatState == false)
		return;

	UUSDFPlayerWarriorAnimInstance* AnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (CurrentComboAttackType == EPlayerWarriorComboType::None)
		{
			ELocomotionState LocomotionState = AnimInstance->GetLocomotionState();
			switch (LocomotionState)
			{
			case ELocomotionState::Idle:
			case ELocomotionState::Walk:
			case ELocomotionState::Run:
			{
				CurrentComboAttackType = EPlayerWarriorComboType::UpperCut;
				FRotator ControllerRotation = GetControlRotation();
				FRotator NewRotation(0.0f, ControllerRotation.Yaw, 0.0f);
				SetActorRotation(NewRotation);

				const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
				const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
				if (!AnimInstance->Montage_IsPlaying(ComboActionData->ComboAttackMontage))
				{
					bAttackState = true;
					Wrapper.OnComboAttackDelegate.ExecuteIfBound();
					ResetCombatStateTime();
				}
			}
			break;
			default:
				break;
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::AttackEKey()
{
	Super::AttackEKey();

	if (bDamagedState)
		return;

	if (bCombatState == false)
		return;

	UUSDFPlayerWarriorAnimInstance* AnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (CurrentComboAttackType == EPlayerWarriorComboType::None)
		{
			ELocomotionState LocomotionState = AnimInstance->GetLocomotionState();
			switch (LocomotionState)
			{
			case ELocomotionState::Idle:
			case ELocomotionState::Walk:
			case ELocomotionState::Run:
			{
				CurrentComboAttackType = EPlayerWarriorComboType::Dash;
				FRotator ControllerRotation = GetControlRotation();
				FRotator NewRotation(0.0f, ControllerRotation.Yaw, 0.0f);
				SetActorRotation(NewRotation);

				const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
				const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
				if (!AnimInstance->Montage_IsPlaying(ComboActionData->ComboAttackMontage))
				{
					bAttackState = true;
					Wrapper.OnComboAttackDelegate.ExecuteIfBound();
					ResetCombatStateTime();
				}
			}
				break;
			default:
				break;
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::AttackRKey()
{
	Super::AttackRKey();

	if (bDamagedState)
		return;

	if (bCombatState == false)
		return;

	UUSDFPlayerWarriorAnimInstance* AnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (CurrentComboAttackType == EPlayerWarriorComboType::None)
		{
			ELocomotionState LocomotionState = AnimInstance->GetLocomotionState();
			switch (LocomotionState)
			{
			case ELocomotionState::Idle:
			case ELocomotionState::Walk:
			case ELocomotionState::Run:
			{
				CurrentComboAttackType = EPlayerWarriorComboType::Power;
				FRotator ControllerRotation = GetControlRotation();
				FRotator NewRotation(0.0f, ControllerRotation.Yaw, 0.0f);
				SetActorRotation(NewRotation);

				const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
				const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
				if (!AnimInstance->Montage_IsPlaying(ComboActionData->ComboAttackMontage))
				{
					PowerAttackAreaLocation = GetActorLocation() + GetActorForwardVector() * 350.0f;
					bAttackState = true;
					Wrapper.OnComboAttackDelegate.ExecuteIfBound();
					ResetCombatStateTime();
				}
			}
				break;
			default:
				break;
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::Dodge()
{
	Super::Dodge();

	UUSDFPlayerWarriorAnimInstance* WarriorAnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());



	if (WarriorAnimInstance)
	{
		if (bDamagedState)
			return;

		if (bAttackState)
			return;

		EPlayerDodgeDirection Direction = EPlayerDodgeDirection::None;

		float X = MovementInputValue.X, Y = MovementInputValue.Y;

		if (X > 0.5f)
		{
			Direction = EPlayerDodgeDirection::Front;
			if (Y < -0.5f)
				Direction = EPlayerDodgeDirection::Front_Left;
			else if (Y > 0.5f)
				Direction = EPlayerDodgeDirection::Front_Right;
		}
		else if (X < -0.5f)
		{
			Direction = EPlayerDodgeDirection::Back;
			if (Y < -0.5f)
				Direction = EPlayerDodgeDirection::Back_Left;
			else if (Y > 0.5f)
				Direction = EPlayerDodgeDirection::Back_Right;
		}
		else if (Y < -0.5f)
			Direction = EPlayerDodgeDirection::Left;
		else if (Y > 0.5f)
			Direction = EPlayerDodgeDirection::Right;

		

		if (Direction != EPlayerDodgeDirection::None)
		{
			if (DodgeAnimMontage.Find(Direction) != nullptr && DodgeAnimMontage[Direction])
			{
				FRotator ControllerRotation = GetControlRotation();
				FRotator NewRotation(0.0f, ControllerRotation.Yaw, 0.0f);
				SetActorRotation(NewRotation);

				WarriorAnimInstance->Montage_Play(DodgeAnimMontage[Direction]);
			}
		}
	}
}

bool AUSDFCharacterPlayerWarrior::PossessCombatStartMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (bCombatState == false)
	{
		if (AnimInstance->Montage_IsPlaying(CombatStartMontage) == true)
			return false;
		
		AnimInstance->Montage_Play(CombatStartMontage, 1.0f);
	
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::CombatStartMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, CombatStartMontage);
		return false;
	}

	return true;
}

void AUSDFCharacterPlayerWarrior::PossessCombatEndMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CombatEndMontage, 1.0f);
}

void AUSDFCharacterPlayerWarrior::PossessAttackMontage()
{
	UUSDFPlayerWarriorAnimInstance* AnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance->Montage_IsPlaying(CombatEndMontage) == true)
		AnimInstance->Montage_Stop(0.2f, CombatEndMontage);

	UStaticMeshComponent* WeaponStaticMesh = WeaponSword->GetMesh();

	if (WeaponStaticMesh->GetAttachSocketName().IsEqual("weapon_socket") == false)
		WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_equiphand_socket");

	if (CurrentComboAttackType == EPlayerWarriorComboType::None)
	{
		ELocomotionState LocomotionState = AnimInstance->GetLocomotionState();
		switch (LocomotionState)
		{
		case ELocomotionState::Idle:
		case ELocomotionState::Walk:
			CurrentComboAttackType = EPlayerWarriorComboType::Default;
			break;
		case ELocomotionState::Run:
			break;
		}
	}

	if (CurrentComboAttackType == EPlayerWarriorComboType::None)
		return;

	FRotator ControllerRotation = GetControlRotation();
	FRotator NewRotation(0.0f, ControllerRotation.Yaw, 0.0f);
	SetActorRotation(NewRotation);

	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
	if (!bAttackState)
	{
		bAttackState = true;
		Wrapper.OnComboAttackDelegate.ExecuteIfBound();
	}
	else
	{
		HasNextComboCommand = true;
	}

	ResetCombatStateTime();
}

void AUSDFCharacterPlayerWarrior::CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("CombatStartMontageEnd Succeed"));
	SetCombatState(true);
}

void AUSDFCharacterPlayerWarrior::EquipWeapon()
{
	UStaticMeshComponent* WeaponStaticMesh = WeaponSword->GetMesh();
	if (WeaponStaticMesh == nullptr)
		return;
	WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_equiphand_socket");
}

void AUSDFCharacterPlayerWarrior::UnEquipWeapon()
{
	UStaticMeshComponent* WeaponStaticMesh = WeaponSword->GetMesh();
	if (WeaponStaticMesh == nullptr)
		return;

	WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_socket");
	SetCombatState(false);
}

void AUSDFCharacterPlayerWarrior::CheckApplyDamagePoint()
{
	switch (CurrentComboAttackType)
	{
		case EPlayerWarriorComboType::Power:
			ApplyDamagePowerAttack();
			break;
		default:
			break;
	}
}

void AUSDFCharacterPlayerWarrior::AttackHitCheck()
{
	switch (CurrentComboAttackType)
	{
		case EPlayerWarriorComboType::Default:
			DefaultAttackHitCheck();
			break;
		case EPlayerWarriorComboType::UpperCut:
			UpperAttackHitCheck();
			break;
		case EPlayerWarriorComboType::Dash:
			DashAttackHitCheck();
			break;
		default:
			break;
	}
}

UParticleSystem* AUSDFCharacterPlayerWarrior::GetTrailParticleSystem()
{
	if (WeaponSword == nullptr)
		return nullptr;

	return WeaponSword->GetWeaponItemData()->GetTrailEffect();
}

FName AUSDFCharacterPlayerWarrior::GetTrailStartSocketName()
{
	if (WeaponSword == nullptr)
		return FName();

	return WeaponSword->GetWeaponItemData()->GetTrailStart();
}

FName AUSDFCharacterPlayerWarrior::GetTrailEndSocketName()
{
	if (WeaponSword == nullptr)
		return FName();

	return WeaponSword->GetWeaponItemData()->GetTrailEnd();
}

bool AUSDFCharacterPlayerWarrior::IsCombatState()
{
	return bCombatState;
}

void AUSDFCharacterPlayerWarrior::CheckCombo()
{
	if (ComboAttackDelegateManager.Find(CurrentComboAttackType) == nullptr)
		return;

	if (HasNextComboCommand)
	{
		const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
		const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];

		if (ComboActionData->MaxComboCount == CurrentComboCount)
			return;

		HitCharaters.Empty();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 0, ComboActionData->MaxComboCount - 1);

		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentComboCount);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionData->ComboAttackMontage);
		HasNextComboCommand = false;
	}
	else
	{
		IgnoreComboCommand = true;
	}
}

void AUSDFCharacterPlayerWarrior::DefaultComboAttack()
{
	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[EPlayerWarriorComboType::Default];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[EPlayerWarriorComboType::Default];

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentComboCount == 0)
	{
		HitCharaters.Empty();

		AnimInstance->Montage_Play(ComboActionData->ComboAttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::ComboActionEnded);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionData->ComboAttackMontage);
	}
	else if (IgnoreComboCommand == false)
	{
		HasNextComboCommand = true;
	}
}

void AUSDFCharacterPlayerWarrior::UpperAttack()
{
	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[EPlayerWarriorComboType::UpperCut];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[EPlayerWarriorComboType::UpperCut];

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	HitCharaters.Empty();
	AnimInstance->Montage_Play(ComboActionData->ComboAttackMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::ComboActionEnded);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionData->ComboAttackMontage);
}

void AUSDFCharacterPlayerWarrior::DashAttack()
{
	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[EPlayerWarriorComboType::Dash];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[EPlayerWarriorComboType::Dash];

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	HitCharaters.Empty();
	AnimInstance->Montage_Play(ComboActionData->ComboAttackMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::ComboActionEnded);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionData->ComboAttackMontage);
}

void AUSDFCharacterPlayerWarrior::PowerAttack()
{
	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[EPlayerWarriorComboType::Power];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[EPlayerWarriorComboType::Power];

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	HitCharaters.Empty();
	AnimInstance->Montage_Play(ComboActionData->ComboAttackMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::ComboActionEnded);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionData->ComboAttackMontage);
}

void AUSDFCharacterPlayerWarrior::DefaultAttackHitCheck()
{
	ExecuteDefaultHitCheck();
}

void AUSDFCharacterPlayerWarrior::UpperAttackHitCheck()
{
	ExecuteDefaultHitCheck();
}

void AUSDFCharacterPlayerWarrior::DashAttackHitCheck()
{
	ExecuteDashHitCheck();
}

void AUSDFCharacterPlayerWarrior::ApplyDamagePowerAttack()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(PowerAttack), false, this);
	float Radius = 350.0f;
	bool bOverlapped = GetWorld()->OverlapMultiByChannel(OverlapResults, PowerAttackAreaLocation, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(Radius), Params);
	
	if (bOverlapped)
	{
		HitCharaters.Empty();
		for (int32 i = 0; i < OverlapResults.Num(); ++i)
		{
			bool bIsExist = false;
			for (TWeakObjectPtr<AUSDFCharacterBase>& Obj : HitCharaters)
			{
				if (!Obj.IsValid())
					continue;

				if (Obj.Get() == OverlapResults[i].GetActor())
				{
					bIsExist = true;
					break;
				}
			}

			AUSDFCharacterBase* HitCharacter = Cast<AUSDFCharacterBase>(OverlapResults[i].GetActor());
			IUSDFDamageableInterface* DamageableTarget = Cast<IUSDFDamageableInterface>(HitCharacter);

			if (HitCharacter&& DamageableTarget && bIsExist == false)
			{
				float DamageAmount = Stat->GetPlayerStat().DefaultAttack;

				FDamageInfo DamageInfo = {};
				DamageInfo.DamageAmount = DamageAmount;
				DamageInfo.DamageCauser = this;
				DamageInfo.DamageType = EDamageType::HitDefault;

				DamageableTarget->TakeDamage(DamageInfo);
				
				int32 AttackHitEffectIndex = FMath::RandRange(0, 2);
				if (AttackHitEffects[AttackHitEffectIndex] != nullptr)
				{
					UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffects[AttackHitEffectIndex], HitCharacter->GetActorLocation(), FRotator(FMath::RandRange(-90, 90), FMath::RandRange(-90, 90), FMath::RandRange(-90, 90)));;
					if (pNiagaraCompo != nullptr)
					{
						pNiagaraCompo->Activate();
					}
				}

				HitCharaters.Add(HitCharacter);
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	//DrawDebugSphere(GetWorld(), PowerAttackAreaLocation, Radius, 20.0f, FColor::Red, false, 5.0f);
#endif
}


void AUSDFCharacterPlayerWarrior::ExecuteDefaultHitCheck()
{

	bool HitDetected = false;
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	FVector AttackBase = FVector::ZeroVector;
	FVector AttackTip = FVector::ZeroVector;

	UStaticMeshComponent* WeaponStaticMesh = WeaponSword->GetMesh();

	if (WeaponStaticMesh)
	{
		AttackBase = WeaponStaticMesh->GetSocketLocation("attack_base");
		AttackTip = WeaponStaticMesh->GetSocketLocation("attack_tip");
	}

	HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(25.0f), Params);
#if ENABLE_DRAW_DEBUG
	/*
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 25.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
	*/
#endif

	if (HitDetected)
	{
		for (auto& HitResult : OutHitResults)
		{
			bool bIsExist = false;
			for (TWeakObjectPtr<AUSDFCharacterBase>& Obj : HitCharaters)
			{
				if (!Obj.IsValid())
					continue;

				if (Obj.Get() == HitResult.GetActor())
				{
					bIsExist = true;
					break;
				}
			}

			AUSDFCharacterBase* HitCharacter = Cast<AUSDFCharacterBase>(HitResult.GetActor());
			IUSDFDamageableInterface* DamageableTarget = Cast<IUSDFDamageableInterface>(HitCharacter);

			if (HitCharacter && DamageableTarget && bIsExist == false)
			{
				float DamageAmount = Stat->GetPlayerStat().DefaultAttack;

				FDamageInfo DamageInfo = {};
				DamageInfo.DamageAmount = DamageAmount;
				DamageInfo.DamageCauser = this;
				DamageInfo.DamageType = EDamageType::HitDefault;

				DamageableTarget->TakeDamage(DamageInfo);
				UAISense_Damage::ReportDamageEvent(this,HitCharacter,this,DamageAmount, HitResult.GetActor()->GetActorLocation(), HitResult.GetActor()->GetActorLocation());

				FVector BoneLocation = FVector::ZeroVector;
				HitCharacter->GetMesh()->FindClosestBone(HitResult.Location, &BoneLocation);
				FVector ImpactNormal = (HitResult.Location - BoneLocation).GetSafeNormal();

				int32 AttackHitEffectIndex = FMath::RandRange(0, 2);
				if (AttackHitEffects[AttackHitEffectIndex] != nullptr)
				{
					UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffects[AttackHitEffectIndex], BoneLocation, FRotationMatrix::MakeFromZ(ImpactNormal).Rotator());
					if (pNiagaraCompo != nullptr)
					{
						pNiagaraCompo->Activate();
					}
				}

				HitCharaters.Add(HitCharacter);
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::ExecuteDashHitCheck()
{
	bool bOverlapped = false;
	TArray<FOverlapResult> OutOverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	float Radius = 120.0f;
	bOverlapped = GetWorld()->OverlapMultiByChannel(OutOverlapResults, GetActorLocation(), FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(Radius), Params);
#if ENABLE_DRAW_DEBUG

	//FColor DrawColor = bOverlapped ? FColor::Green : FColor::Red;

	//DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 20, DrawColor, false, 5.0f);

#endif

	if (bOverlapped)
	{
		for (auto& OverlapResult : OutOverlapResults)
		{
			bool bIsExist = false;
			for (TWeakObjectPtr<AUSDFCharacterBase>& Obj : HitCharaters)
			{
				if (!Obj.IsValid())
					continue;

				if (Obj.Get() == OverlapResult.GetActor())
				{
					bIsExist = true;
					break;
				}
			}

			AUSDFCharacterBase* HitCharacter = Cast<AUSDFCharacterBase>(OverlapResult.GetActor());
			IUSDFDamageableInterface* DamageableTarget = Cast<IUSDFDamageableInterface>(HitCharacter);

			if (HitCharacter && DamageableTarget && bIsExist == false)
			{
				float DamageAmount = Stat->GetPlayerStat().DefaultAttack;

				FDamageInfo DamageInfo = {};
				DamageInfo.DamageAmount = DamageAmount;
				DamageInfo.DamageCauser = this;
				DamageInfo.DamageType = EDamageType::HitDefault;

				DamageableTarget->TakeDamage(DamageInfo);

				FVector BoneLocation = FVector::ZeroVector;
				HitCharacter->GetMesh()->FindClosestBone(OverlapResult.GetActor()->GetActorLocation(), &BoneLocation);
				FVector ImpactNormal = (OverlapResult.GetActor()->GetActorLocation() - BoneLocation).GetSafeNormal();

				int32 AttackHitEffectIndex = FMath::RandRange(0, 2);
				if (AttackHitEffects[AttackHitEffectIndex] != nullptr)
				{
					UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffects[AttackHitEffectIndex], BoneLocation, FRotationMatrix::MakeFromZ(ImpactNormal).Rotator());
					if (pNiagaraCompo != nullptr)
					{
						pNiagaraCompo->Activate();
					}
				}

				HitCharaters.Add(HitCharacter);
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::ResetCombatStateTime()
{
	CombatStateTime = 10;
}

void AUSDFCharacterPlayerWarrior::OnDamageResponse(FDamageInfo DamageInfo)
{
	Super::OnDamageResponse(DamageInfo);
}


void AUSDFCharacterPlayerWarrior::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_USDF_PlAYER_CAPSULE);
	HasNextComboCommand = false;
	IgnoreComboCommand = false;
	CurrentComboCount = 0;
	CurrentComboAttackType = EPlayerWarriorComboType::None;
	bAttackState = false;
	HitCharaters.Empty();

	MakeNoise(1.0f, this, GetActorLocation());
}


void AUSDFCharacterPlayerWarrior::SetCombatState(bool NewCombatState)
{
	bCombatState = NewCombatState;

	if(NewCombatState == true)
		CombatStateTime = 10;
}