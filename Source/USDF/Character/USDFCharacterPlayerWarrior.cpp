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
#include "Interface/USDFCharacterHitReactInterface.h"
#include "CharacterStat/USDFPlayerStatComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "USDFCharacterControlData.h"
#include "Components/SphereComponent.h"
#include "Animation/USDFLocomotionState.h"
#include "Item/USDFItemWeaponDarkSword.h"
#include "Item/USDFWeaponItemData.h"

AUSDFCharacterPlayerWarrior::AUSDFCharacterPlayerWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectSphere->SetSphereRadius(350.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/IdaFaber/Meshes/Girl/SK_CALISTA_01.SK_CALISTA_01"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/Player/ABP_USDF_PlayerWarrior_RM.ABP_USDF_PlayerWarrior_RM_C"));
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

	static ConstructorHelpers::FClassFinder<AUSDFItemWeapon> WeaponSwordClassRef(TEXT("/Game/Item/BP_USDFItemWeaponDarkSword.BP_USDFItemWeaponDarkSword_C"));
	if (WeaponSwordClassRef.Class)
	{
		WeaponSwordClass = WeaponSwordClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> DefaultComboAttackDataRef(TEXT("/Game/CharacterAction/USDF_ComboAttackData"));
	if (DefaultComboAttackDataRef.Object)
	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::DefaultComboAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::Default, Wrapper);
		ComboAttackDataManager.Add(EPlayerWarriorComboType::Default, DefaultComboAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> UpperAttackDataRef(TEXT("/Game/CharacterAction/USDF_UpperAttackData.USDF_UpperAttackData"));
	if (UpperAttackDataRef.Object)
	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::UpperAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::UpperCut, Wrapper);
		ComboAttackDataManager.Add(EPlayerWarriorComboType::UpperCut, UpperAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> DashAttackDataRef(TEXT("/Game/CharacterAction/USDF_DashAttackData.USDF_DashAttackData"));
	if (DashAttackDataRef.Object)
	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::DashAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::Dash, Wrapper);
		ComboAttackDataManager.Add(EPlayerWarriorComboType::Dash, DashAttackDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> PowerAttackDataRef(TEXT("/Game/CharacterAction/USDF_PowerAttackData.USDF_PowerAttackData"));
	if (PowerAttackDataRef.Object)
	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::PowerAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::Power, Wrapper);
		ComboAttackDataManager.Add(EPlayerWarriorComboType::Power, PowerAttackDataRef.Object);
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

	LandedDelegate.AddDynamic(this,&AUSDFCharacterPlayerWarrior::OnWarriorLanded);
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
			UE_LOG(LogTemp, Display, TEXT("CombateEndMontageStart"));
			PossessCombatEndMontage();
		}
	}

	if (bUpperHit == true)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, 1200, DeltaSeconds, 2.0f);
		UpperHitStateTime -= DeltaSeconds;
		if (UpperHitStateTime <= 0)
			bUpperHit = false;
	}
	else
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CharacterControlManager[CurrentControlType]->TargetArmLength, DeltaSeconds, 2.0f);
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
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayerWarrior::WarriorJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::WarriorStopJumping);
}

void AUSDFCharacterPlayerWarrior::Attack()
{
	Super::Attack();

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
			{
				CurrentComboAttackType = EPlayerWarriorComboType::UpperCut;
				const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
				const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
				if (!AnimInstance->Montage_IsPlaying(ComboActionData->ComboAttackMontage))
				{
					RotateToTarget(EHitReactType::Upper);

					bAttackState = true;
					Wrapper.OnComboAttackDelegate.ExecuteIfBound();
					ResetCombatStateTime();
				}
			}
			break;
			case ELocomotionState::Run:
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
				break;
			case ELocomotionState::Run:
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
			{
				CurrentComboAttackType = EPlayerWarriorComboType::Power;
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
			case ELocomotionState::Run:
				break;
			default:
				break;
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::WarriorJump()
{
	UUSDFPlayerWarriorAnimInstance* WarriorAnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());

	if (WarriorAnimInstance)
	{
		if (bAttackState)
			return;

		Jump();

		WarriorAnimInstance->K2_OnJump();

		WarriorAnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
}

void AUSDFCharacterPlayerWarrior::WarriorStopJumping()
{
	StopJumping();
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
			CurrentComboAttackType = EPlayerWarriorComboType::Dash;
			DetectSphere->SetSphereRadius(800.0f);
			break;
		case ELocomotionState::Jumping:
			break;
		}
	}

	if (CurrentComboAttackType == EPlayerWarriorComboType::None)
		return;

	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
	if (!bAttackState)
	{
		EHitReactType HitReactType = EHitReactType::None;
		switch (CurrentComboAttackType)
		{
			case  EPlayerWarriorComboType::Default:
				HitReactType = EHitReactType::Default;
				break;
			case EPlayerWarriorComboType::UpperCut:
				HitReactType = EHitReactType::Upper;
				break;
			case EPlayerWarriorComboType::Dash:
				HitReactType = EHitReactType::Dash;
				break;
			case EPlayerWarriorComboType::Power:
				HitReactType = EHitReactType::PowerAttack;
				break;
			default:
				break;
		}
		RotateToTarget(HitReactType);

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
	Super::CheckApplyDamagePoint();
	
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
	Super::AttackHitCheck();

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

void AUSDFCharacterPlayerWarrior::OnWarriorLanded(const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("Landed succeed"));
	UUSDFPlayerWarriorAnimInstance* WarriorAnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());
	if (WarriorAnimInstance)
	{
		WarriorAnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	}
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
	ExecuteDefaultHitCheck(EHitReactType::Default);
}

void AUSDFCharacterPlayerWarrior::UpperAttackHitCheck()
{
	ExecuteDefaultHitCheck(EHitReactType::Upper);
}

void AUSDFCharacterPlayerWarrior::DashAttackHitCheck()
{
	ExecuteDefaultHitCheck(EHitReactType::Dash);
}

void AUSDFCharacterPlayerWarrior::ApplyDamagePowerAttack()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(PowerAttack), false, this);
	float Radius = 350.0f;
	bool bOverlapped = GetWorld()->OverlapMultiByChannel(OverlapResults, PowerAttackAreaLocation, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(Radius), Params);
	
	if (bOverlapped)
	{
		UE_LOG(LogTemp, Display, TEXT("Power Attack hit Count : %d"), OverlapResults.Num());
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

			if (HitCharacter && bIsExist == false)
			{
				float DamageAmount = 10;
				FDamageEvent DamageEvent;
				UE_LOG(LogTemp, Display, TEXT("Power Attack hit"));
				HitCharacter->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
				int32 AttackHitEffectIndex = FMath::RandRange(0, 2);
				if (AttackHitEffects[AttackHitEffectIndex] != nullptr)
				{
					UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffects[AttackHitEffectIndex], HitCharacter->GetActorLocation(), FRotator(FMath::RandRange(-90, 90), FMath::RandRange(-90, 90), FMath::RandRange(-90, 90)));;
					if (pNiagaraCompo != nullptr)
					{
						pNiagaraCompo->Activate();
					}
				}

				IUSDFCharacterHitReactInterface* HitReactableCharacter = Cast<IUSDFCharacterHitReactInterface>(HitCharacter);
				if (HitReactableCharacter)
				{
					HitReactableCharacter->HitReact(DamageAmount, EHitReactType::PowerAttack, this);
				}

				HitCharaters.Add(HitCharacter);
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), PowerAttackAreaLocation, Radius, 20.0f, FColor::Red, false, 5.0f);
#endif
}


void AUSDFCharacterPlayerWarrior::ExecuteDefaultHitCheck(EHitReactType HitReactType)
{

	bool HitDetected = false;
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	FVector AttackBase = FVector::ZeroVector;
	FVector AttackTip = FVector::ZeroVector;

	UStaticMeshComponent* WeaponStaticMesh = WeaponSword->GetMesh();
	switch (HitReactType)
	{
		case EHitReactType::Default:
		case EHitReactType::Dash:
		{
			if (WeaponStaticMesh)
			{
				AttackBase = WeaponStaticMesh->GetSocketLocation("attack_base");
				AttackTip = WeaponStaticMesh->GetSocketLocation("attack_tip");
			}

			HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(25.0f), Params);
#if ENABLE_DRAW_DEBUG
			FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
			float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
			FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

			DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 25.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
		}
			break;
		case EHitReactType::Upper:
		{
			if (WeaponStaticMesh)
			{
				AttackBase = WeaponStaticMesh->GetSocketLocation("attack_base");
				AttackTip = WeaponStaticMesh->GetSocketLocation("attack_tip");
			}

			HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(25.0f), Params);
#if ENABLE_DRAW_DEBUG
			FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
			float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
			FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

			DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 25.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
		}
			break;
	}

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

			if (HitCharacter && bIsExist == false)
			{
				float DamageAmount = Stat->GetPlayerStat().DefaultAttack;
				FDamageEvent DamageEvent;

				HitCharacter->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
				HitCharaters.Add(HitCharacter);
				int32 AttackHitEffectIndex = FMath::RandRange(0, 2);
				if (AttackHitEffects[AttackHitEffectIndex] != nullptr)
				{
					UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffects[AttackHitEffectIndex], HitResult.Location, FRotationMatrix::MakeFromZ(HitResult.ImpactNormal).Rotator());
					if (pNiagaraCompo != nullptr)
					{
						pNiagaraCompo->Activate();
					}
				}

				IUSDFCharacterHitReactInterface* HitReactableCharacter = Cast<IUSDFCharacterHitReactInterface>(HitCharacter);
				if (HitReactableCharacter)
				{
					HitReactableCharacter->HitReact(DamageAmount, HitReactType, this);
				}
			}
		}
	}
}

void AUSDFCharacterPlayerWarrior::ResetCombatStateTime()
{
	CombatStateTime = 10;
}

void AUSDFCharacterPlayerWarrior::RotateToTarget(EHitReactType HitReactType)
{
	Super::RotateToTarget(HitReactType);

	DetectSphere->SetSphereRadius(350.0f);
}

void AUSDFCharacterPlayerWarrior::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("Montage End"));
	HasNextComboCommand = false;
	IgnoreComboCommand = false;
	CurrentComboCount = 0;
	CurrentComboAttackType = EPlayerWarriorComboType::None;
	bAttackState = false;
	HitCharaters.Empty();
}

void AUSDFCharacterPlayerWarrior::HitReact(const float DamageAmount, EHitReactType HitReactType, const AActor* HitCauser)
{
	
}

bool AUSDFCharacterPlayerWarrior::GetHitReactState()
{
	return bHitReactState;
}

void AUSDFCharacterPlayerWarrior::SetCombatState(bool NewCombatState)
{
	bCombatState = NewCombatState;
	
	UE_LOG(LogTemp, Display, TEXT("Combat State : %d"), bCombatState);

	if(NewCombatState == true)
		CombatStateTime = 10;
}