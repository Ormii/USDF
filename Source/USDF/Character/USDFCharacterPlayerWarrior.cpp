// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterPlayerWarrior.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Animation/USDFPlayerWarriorAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/USDFComboActionData.h"
#include "Physics/USDFCollision.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AUSDFCharacterPlayerWarrior::AUSDFCharacterPlayerWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	WeaponStaticMesh->SetupAttachment(GetMesh(), "weapon_socket");
	WeaponStaticMesh->SetCollisionProfileName("NoCollision");

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(TEXT("/Game/ReferenceAsset/Swords/Models/SM_Sword_4.SM_Sword_4"));
	if (WeaponMeshRef.Object)
	{
		WeaponStaticMesh->SetStaticMesh(WeaponMeshRef.Object);
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

	for (int i = 0; i < 3; ++i)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackHitEffectRef(*FString::Printf(TEXT("/Game/ReferenceAsset/RealisticBlood/Slash/Niagara/NS_Slash_%d.NS_Slash_%d"),i,i));
		if (AttackHitEffectRef.Object)
		{
			AttackHitEffects.Add(AttackHitEffectRef.Object);
		}
	}

	CurrentComboAttackType = EPlayerWarriorComboType::Default;
}

void AUSDFCharacterPlayerWarrior::BeginPlay()
{
	Super::BeginPlay();
	LandedDelegate.AddDynamic(this,&AUSDFCharacterPlayerWarrior::OnWarriorLanded);
}

void AUSDFCharacterPlayerWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->SetMaxHp(200.0f);
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
}

void AUSDFCharacterPlayerWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AUSDFCharacterPlayerWarrior::Attack);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayerWarrior::WarriorJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::WarriorStopJumping);
}

void AUSDFCharacterPlayerWarrior::Attack()
{
	Super::Attack();

	UE_LOG(LogTemp, Display, TEXT("AttackStart"));
	
	PossessCombatStartMontage();
}

void AUSDFCharacterPlayerWarrior::WarriorJump()
{
	UUSDFPlayerWarriorAnimInstance* WarriorAnimInstance = Cast<UUSDFPlayerWarriorAnimInstance>(GetMesh()->GetAnimInstance());

	if (WarriorAnimInstance)
	{
		const UUSDFComboActionData* DefaultComboActionData = ComboAttackDataManager[EPlayerWarriorComboType::Default];

		if (WarriorAnimInstance->Montage_IsPlaying(DefaultComboActionData->ComboAttackMontage) == true)
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

	if (WeaponStaticMesh->GetAttachSocketName().IsEqual("weapon_socket") == false)
		WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_equiphand_socket");
	
	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
	if (!AnimInstance->Montage_IsPlaying(ComboActionData->ComboAttackMontage))
	{
		Wrapper.OnComboAttackDelegate.Execute(CurrentComboCount);
	}
	else
	{
		HasNextComboCommand = true;
	}

	CombatStateTime = 10;
}

void AUSDFCharacterPlayerWarrior::PossessCombatEndMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CombatEndMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::CombatEndMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CombatEndMontage);

	bAttackState = true;
}

void AUSDFCharacterPlayerWarrior::CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("CombatStartMontageEnd Succeed"));
	SetCombatState(true);
}

void AUSDFCharacterPlayerWarrior::CombatEndMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{

}

void AUSDFCharacterPlayerWarrior::EquipWeapon()
{
	Super::EquipWeapon();
	WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_equiphand_socket");
}

void AUSDFCharacterPlayerWarrior::UnEquipWeapon()
{
	Super::EquipWeapon();
	WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_socket");
	SetCombatState(false);
}

void AUSDFCharacterPlayerWarrior::AttackHitCheck()
{
	Super::AttackHitCheck();

	FVector AttackBase = FVector::ZeroVector;
	FVector AttackTip = FVector::ZeroVector;

	if (WeaponStaticMesh)
	{
		AttackBase = WeaponStaticMesh->GetSocketLocation("attack_base");
		AttackTip = WeaponStaticMesh->GetSocketLocation("attack_tip");
	}

	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDFACTION, FCollisionShape::MakeSphere(5.0f), Params);

	if (HitDetected)
	{
		for (auto HitResult : OutHitResults)
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
				float DamageAmount = 40;
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
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 5.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
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

bool AUSDFCharacterPlayerWarrior::CheckCombo()
{
	Super::CheckCombo();

	UE_LOG(LogTemp, Display, TEXT("CheckCombo"));

	if (HasNextComboCommand)
	{
		UE_LOG(LogTemp, Display, TEXT("HaxNextComboCommand"));
		const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
		const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];

		if (ComboActionData->MaxComboCount == CurrentComboCount)
			return false;

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

	return false;
}

void AUSDFCharacterPlayerWarrior::DefaultComboAttack(int32 NextCombo)
{
	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[EPlayerWarriorComboType::Default];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[EPlayerWarriorComboType::Default];

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentComboCount == 0)
	{
		HitCharaters.Empty();

		CurrentComboAttackType = EPlayerWarriorComboType::Default;
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

void AUSDFCharacterPlayerWarrior::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	HasNextComboCommand = false;
	IgnoreComboCommand = false;
	CurrentComboCount = 0;

	bAttackState = false;
	HitCharaters.Empty();
}

void AUSDFCharacterPlayerWarrior::SetCombatState(bool NewCombatState)
{
	Super::SetCombatState(NewCombatState);
	
	UE_LOG(LogTemp, Display, TEXT("Combat State : %d"), bCombatState);

	if(NewCombatState == true)
		CombatStateTime = 10;

}