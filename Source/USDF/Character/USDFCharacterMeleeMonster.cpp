// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterMeleeMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "Physics/USDFCollision.h"
#include "AI/USDFMeleeMonsterAIController.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CharacterStat/USDFNormalMonsterStatComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"
#include "Character/USDFCharacterPlayer.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 380.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactFrontMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonster_HitReact_Front.BS_USDF_MeleeMonster_HitReact_Front"));
	if (HitReactFrontMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Front, HitReactFrontMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactBackMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonster_HitReact_Back.BS_USDF_MeleeMonster_HitReact_Back"));
	if (HitReactBackMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Back, HitReactBackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactRightMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonster_HitReact_Right.BS_USDF_MeleeMonster_HitReact_Right"));
	if (HitReactRightMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Right, HitReactRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactLeftMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonster_HitReact_Left.BS_USDF_MeleeMonster_HitReact_Left"));
	if (HitReactLeftMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Left, HitReactLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeakAttackMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonsterWeakAttack.BS_USDF_MeleeMonsterWeakAttack"));
	if (WeakAttackMontageRef.Object)
	{
		ActionMontages.Add(EMeleeMonsterActionType::WeakAttack, WeakAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StrongAttackMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonster_StrongAttack.BS_USDF_MeleeMonster_StrongAttack"));
	if (StrongAttackMontageRef.Object)
	{
		ActionMontages.Add(EMeleeMonsterActionType::StrongAttack, StrongAttackMontageRef.Object);
	}


	for (int i = 0; i < 3; ++i)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackHitEffectRef(*FString::Printf(TEXT("/Game/ReferenceAsset/RealisticBlood/Slash/Niagara/NS_Slash_%d.NS_Slash_%d"), i, i));
		if (AttackHitEffectRef.Object)
		{
			AttackHitEffects.Add(AttackHitEffectRef.Object);
		}
	}

	CurrentActionType = EMeleeMonsterActionType::None;
}

void AUSDFCharacterMeleeMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUSDFGameSingleton* GameSingleton = Cast<UUSDFGameSingleton>(GEngine->GameSingleton.Get());

	Stat->InitNormalMonsterStat(GameSingleton->GetNormalMonsterStat("MeleeMonster"));
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetNormalMonsterStat().RunSpeed;
}

void AUSDFCharacterMeleeMonster::ActionByAI(EAIActionType InAIActionType)
{
	switch (InAIActionType)
	{
		case EAIActionType::Melee:
		{
			int32 RandValue = FMath::RandRange(0, 100);
			if (RandValue >= 80)
				CurrentActionType = EMeleeMonsterActionType::StrongAttack;
			else
				CurrentActionType = EMeleeMonsterActionType::WeakAttack;
		}
			break;
		default:
			break;
	}

	if (CurrentActionType != EMeleeMonsterActionType::None)
	{
		UUSDFNonPlayerAnimInstance* AnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			UAnimMontage* PlayAttackMontage = ActionMontages[CurrentActionType];

			HitCharaters.Empty();
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(PlayAttackMontage);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &AUSDFCharacterMeleeMonster::AttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(OnMontageEnded, PlayAttackMontage);
		}
	}
}

void AUSDFCharacterMeleeMonster::ActionFinished()
{
	Super::ActionFinished();
	CurrentActionType = EMeleeMonsterActionType::None;
	HitCharaters.Empty();
}

void AUSDFCharacterMeleeMonster::AttackHitCheck()
{
	switch (CurrentActionType)
	{
		case EMeleeMonsterActionType::WeakAttack:
			WeakAttackHitCheck();
			break;
		case EMeleeMonsterActionType::StrongAttack:
			StrongAttackHitCheck();
			break;
	}
}

void AUSDFCharacterMeleeMonster::AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ActionFinished();
}

void AUSDFCharacterMeleeMonster::WeakAttackHitCheck()
{

	FVector AttackBase = GetMesh()->GetSocketLocation("attack_start_hand_r");
	FVector AttackTip = GetMesh()->GetSocketLocation("attack_end_hand_r");

	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_NONPLAYERACTION, FCollisionShape::MakeSphere(15.0f), Params);

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

			AUSDFCharacterPlayer* HitCharacter = Cast<AUSDFCharacterPlayer>(HitResult.GetActor());
			if (HitCharacter == nullptr)
				continue;

			IUSDFDamageableInterface* DamageableTarget = Cast<IUSDFDamageableInterface>(HitCharacter);

			if (DamageableTarget && bIsExist == false)
			{
				float DamageAmount = Stat->GetNormalMonsterStat().DefaultAttack;

				FDamageInfo DamageInfo = {};
				DamageInfo.DamageAmount = DamageAmount;
				DamageInfo.DamageCauser = this;
				DamageInfo.DamageType = EDamageType::HitDefault;

				DamageableTarget->TakeDamage(DamageInfo);

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

#if ENABLE_DRAW_DEBUG
	//FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	//float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 15.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
}

void AUSDFCharacterMeleeMonster::StrongAttackHitCheck()
{

	FVector AttackBase = GetMesh()->GetSocketLocation("attack_start_hand_l");
	FVector AttackTip = GetMesh()->GetSocketLocation("attack_end_hand_l");

	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, AttackBase, AttackTip, FQuat::Identity, CCHANNEL_USDF_NONPLAYERACTION, FCollisionShape::MakeSphere(15.0f), Params);

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

			AUSDFCharacterPlayer* HitCharacter = Cast<AUSDFCharacterPlayer>(HitResult.GetActor());
			if (HitCharacter == nullptr)
				continue;
			IUSDFDamageableInterface* DamageableTarget = Cast<IUSDFDamageableInterface>(HitCharacter);

			if (DamageableTarget && bIsExist == false)
			{
				float DamageAmount = Stat->GetNormalMonsterStat().StrongAttack;

				FDamageInfo DamageInfo = {};
				DamageInfo.DamageAmount = DamageAmount;
				DamageInfo.DamageCauser = this;
				DamageInfo.DamageType = EDamageType::HitDefault;

				DamageableTarget->TakeDamage(DamageInfo);

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

#if ENABLE_DRAW_DEBUG
	/*
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 15.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
	*/
#endif
}

void AUSDFCharacterMeleeMonster::OnDeath()
{
	Super::OnDeath();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		Destroy();
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 5.0f, false);
}
