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
#include "Interface/USDFCharacterAIInterface.h"

AUSDFCharacterPlayerWarrior::AUSDFCharacterPlayerWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	WeaponStaticMesh->SetupAttachment(GetMesh(), "weapon_socket");
	WeaponStaticMesh->SetCollisionProfileName("NoCollision");

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

	static ConstructorHelpers::FObjectFinder<UUSDFComboActionData> UpperAttackDataRef(TEXT("/Game/CharacterAction/USDF_UpperAttackData.USDF_UpperAttackData"));
	if (UpperAttackDataRef.Object)
	{
		FComboAttackDelegate NewComboAttackDelegate;
		NewComboAttackDelegate.BindUObject(this, &AUSDFCharacterPlayerWarrior::UpperAttack);

		FComboAttackDelegateWrapper Wrapper(NewComboAttackDelegate);
		ComboAttackDelegateManager.Add(EPlayerWarriorComboType::UpperCut, Wrapper);
		ComboAttackDataManager.Add(EPlayerWarriorComboType::UpperCut, UpperAttackDataRef.Object);
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
}

void AUSDFCharacterPlayerWarrior::BeginPlay()
{
	Super::BeginPlay();
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
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUSDFCharacterPlayerWarrior::WarriorJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUSDFCharacterPlayerWarrior::WarriorStopJumping);
}

void AUSDFCharacterPlayerWarrior::Attack()
{
	Super::Attack();

	if (PossessCombatStartMontage())
	{
		UE_LOG(LogTemp, Display, TEXT("AttackStart"));
		PossessAttackMontage();
	}
}

void AUSDFCharacterPlayerWarrior::ReleaseAttack()
{
	Super::ReleaseAttack();
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance->Montage_IsPlaying(CombatEndMontage) == true)
		AnimInstance->Montage_Stop(0.2f, CombatEndMontage);

	if (WeaponStaticMesh->GetAttachSocketName().IsEqual("weapon_socket") == false)
		WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_equiphand_socket");

	UE_LOG(LogTemp, Display, TEXT("AttackKeyPressTime : %f"), AttackKeyPressTime);
	if (AttackKeyPressTime <= 0.2f)
	{
		CurrentComboAttackType = EPlayerWarriorComboType::Default;
	}
	else
	{
		CurrentComboAttackType = EPlayerWarriorComboType::UpperCut;
	}

	const FComboAttackDelegateWrapper& Wrapper = ComboAttackDelegateManager[CurrentComboAttackType];
	const UUSDFComboActionData* ComboActionData = ComboAttackDataManager[CurrentComboAttackType];
	if (!AnimInstance->Montage_IsPlaying(ComboActionData->ComboAttackMontage))
	{
		// 타겟 설정
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams OverlapParams(SCENE_QUERY_STAT(FindEnemy), false, this);
		FVector Center = GetActorLocation();
		float Radius = DetectSphere->GetScaledSphereRadius();

		bool bOverlapped = GetWorld()->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(Radius), OverlapParams);
		if (bOverlapped)
		{
			OverlapResults.Sort([&](const FOverlapResult& LHS, const FOverlapResult& RHS)-> bool{
				FVector LHSForwardVector = LHS.GetActor()->GetActorLocation() - Center;
				FVector RHSForwardVector = RHS.GetActor()->GetActorLocation() - Center;

				return LHSForwardVector.Length() < RHSForwardVector.Length();
			});

			
#if ENABLE_DRAW_DEBUG
			for (int32 i = 0; i < OverlapResults.Num(); ++i)
			{
				IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(OverlapResults[i].GetActor());
				if (AIPawn == nullptr)
					continue;

				DrawDebugPoint(GetWorld(), OverlapResults[i].GetActor()->GetActorLocation(), 10.0f, FColor::Red, false, 0.2f);
				DrawDebugLine(GetWorld(), Center, OverlapResults[i].GetActor()->GetActorLocation(), FColor::Red, false, 0.27f);
			}
#endif
			for (int32 i = 0; i < OverlapResults.Num(); ++i)
			{
				IUSDFCharacterAIInterface* AIPawn = Cast<IUSDFCharacterAIInterface>(OverlapResults[i].GetActor());
				if (AIPawn == nullptr)
					continue;

				FVector TargetForwardVector = OverlapResults[i].GetActor()->GetActorLocation() - Center;
				TargetForwardVector = TargetForwardVector.GetSafeNormal();

				FHitResult HitResult;
				float HitRadius = 45;
				FCollisionQueryParams HitParams(SCENE_QUERY_STAT(LookDirect), false, this);
				FVector StartPoint = Center + TargetForwardVector * HitRadius;
				FVector EndPoint = StartPoint + TargetForwardVector *(Radius - 2 * HitRadius);
				float HalfLength = (Radius - 2 * HitRadius) * 0.5f;

				bool bHitted = GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FRotationMatrix::MakeFromZ(TargetForwardVector).ToQuat(), ECC_Pawn, FCollisionShape::MakeSphere(HitRadius), HitParams);
				if (bHitted)
				{
					AIPawn = Cast<IUSDFCharacterAIInterface>(HitResult.GetActor());
					if (AIPawn == nullptr)
						continue;

					FRotator Rotation = FRotationMatrix::MakeFromX(TargetForwardVector).Rotator();
					FRotator NewRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
					SetActorRotation(NewRotation);

#if ENABLE_DRAW_DEBUG
					DrawDebugCapsule(GetWorld(), (StartPoint + EndPoint) / 2, HalfLength, HitRadius, FRotationMatrix::MakeFromZ(TargetForwardVector).ToQuat(), FColor::Green, false, 0.27f);
					DrawDebugLine(GetWorld(), Center, OverlapResults[i].GetActor()->GetActorLocation(), FColor::Green, false, 0.27f);
#endif
					break;
				}
			}
		}

		bAttackState = true;
		Wrapper.OnComboAttackDelegate.ExecuteIfBound();
	}
	else
	{
		HasNextComboCommand = true;
	}

	CombatStateTime = 10;
}

void AUSDFCharacterPlayerWarrior::CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("CombatStartMontageEnd Succeed"));
	SetCombatState(true);
}

void AUSDFCharacterPlayerWarrior::EquipWeapon()
{
	WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_equiphand_socket");
}

void AUSDFCharacterPlayerWarrior::UnEquipWeapon()
{
	WeaponStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_socket");
	SetCombatState(false);
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
		default:
			break;
	}
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

void AUSDFCharacterPlayerWarrior::DefaultAttackHitCheck()
{
	FVector AttackBase = FVector::ZeroVector;
	FVector AttackTip = FVector::ZeroVector;

	if (WeaponStaticMesh)
	{
		AttackBase = WeaponStaticMesh->GetSocketLocation("attack_base");
		AttackTip = WeaponStaticMesh->GetSocketLocation("attack_tip");
	}

	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(25.0f), Params);

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
					HitReactableCharacter->HitReact(HitResult, DamageAmount, EHitReactType::Default, this);
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 25.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
}

void AUSDFCharacterPlayerWarrior::UpperAttackHitCheck()
{
	FVector AttackBase = FVector::ZeroVector;
	FVector AttackTip = FVector::ZeroVector;

	if (WeaponStaticMesh)
	{
		AttackBase = WeaponStaticMesh->GetSocketLocation("attack_base");
		AttackTip = WeaponStaticMesh->GetSocketLocation("attack_tip");
	}

	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_PLAYERACTION, FCollisionShape::MakeSphere(25.0f), Params);

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
				bUpperHit = true;
				UpperHitStateTime = 2.0f;
				UE_LOG(LogTemp, Display, TEXT("Velocity x: %f, y: %f, z:%f"), HitCharacter->GetCharacterMovement()->Velocity.X, HitCharacter->GetCharacterMovement()->Velocity.Y, HitCharacter->GetCharacterMovement()->Velocity.Z);
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
					HitReactableCharacter->HitReact(HitResult, DamageAmount, EHitReactType::Upper, this);
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 25.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
}

void AUSDFCharacterPlayerWarrior::ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		if (CurrentMontage != nullptr && CurrentMontage->GetGroupName().IsEqual("AttackGroup"))
			return;
	}

	HasNextComboCommand = false;
	IgnoreComboCommand = false;
	CurrentComboCount = 0;
	CurrentComboAttackType = EPlayerWarriorComboType::None;
	bAttackState = false;
	HitCharaters.Empty();
}

void AUSDFCharacterPlayerWarrior::HitReact(const FHitResult& HitResult, const float DamageAmount, EHitReactType HitReactType, const AActor* HitCauser)
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