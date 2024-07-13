// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterMeleeMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/USDFMeleeMonsterAnimInstance.h"
#include "Physics/USDFCollision.h"
#include "AI/USDFMeleeMonsterAIController.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CharacterStat/USDFNormalMonsterStatComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "Interface/USDFGameModeInterface.h"
#include "GameFramework/GameModeBase.h"

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
		AttackMontages.Add(EMeleeMonsterAttackType::WeakAttack, WeakAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StrongAttackMontageRef(TEXT("/Game/Animation/NonPlayer/MeleeMonster/BS_USDF_MeleeMonster_StrongAttack.BS_USDF_MeleeMonster_StrongAttack"));
	if (StrongAttackMontageRef.Object)
	{
		AttackMontages.Add(EMeleeMonsterAttackType::StrongAttack, StrongAttackMontageRef.Object);
	}


	for (int i = 0; i < 3; ++i)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackHitEffectRef(*FString::Printf(TEXT("/Game/ReferenceAsset/RealisticBlood/Slash/Niagara/NS_Slash_%d.NS_Slash_%d"), i, i));
		if (AttackHitEffectRef.Object)
		{
			AttackHitEffects.Add(AttackHitEffectRef.Object);
		}
	}

	CurrentAttackType = EMeleeMonsterAttackType::None;
}

void AUSDFCharacterMeleeMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUSDFGameSingleton* GameSingleton = Cast<UUSDFGameSingleton>(GEngine->GameSingleton.Get());

	Stat->InitNormalMonsterStat(GameSingleton->GetNormalMonsterStat("MeleeMonster"));
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetNormalMonsterStat().RunSpeed;
}

void AUSDFCharacterMeleeMonster::AttackByAI(EAIAttackType InAIAttackType)
{
	UE_LOG(LogTemp, Display, TEXT("AI AttackStart"));
	switch (InAIAttackType)
	{
		case EAIAttackType::Melee:
		{
			int32 RandValue = FMath::RandRange(0, 100);
			if (RandValue >= 80)
				CurrentAttackType = EMeleeMonsterAttackType::StrongAttack;
			else
				CurrentAttackType = EMeleeMonsterAttackType::WeakAttack;
		}
			break;
		case EAIAttackType::Range:
			break;
		case EAIAttackType::Dash:
			break;
	}

	if (CurrentAttackType != EMeleeMonsterAttackType::None)
	{
		UUSDFMeleeMonsterAnimInstance* AnimInstance = Cast<UUSDFMeleeMonsterAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			UAnimMontage* PlayAttackMontage = AttackMontages[CurrentAttackType];

			HitCharaters.Empty();
			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(PlayAttackMontage);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &AUSDFCharacterMeleeMonster::AttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(OnMontageEnded, PlayAttackMontage);
		}
	}
}

void AUSDFCharacterMeleeMonster::AttackFinished()
{
	Super::AttackFinished();
	CurrentAttackType = EMeleeMonsterAttackType::None;
	HitCharaters.Empty();
}

void AUSDFCharacterMeleeMonster::HitReact(const float DamageAmount, EHitReactType HitReactType ,const AActor* HitCauser)
{
	Super::HitReact(DamageAmount, HitReactType, HitCauser);
	CurrentAttackType = EMeleeMonsterAttackType::None;
}

void AUSDFCharacterMeleeMonster::AttackHitCheck()
{
	switch (CurrentAttackType)
	{
		case EMeleeMonsterAttackType::WeakAttack:
			WeakAttackHitCheck();
			break;
		case EMeleeMonsterAttackType::StrongAttack:
			StrongAttackHitCheck();
			break;
	}

}

void AUSDFCharacterMeleeMonster::SetDead()
{
	Super::SetDead();


	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]() {
			Destroy();
			IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
			if (GameModeInterface)
			{
				GameModeInterface->UpdateCurrent(StaticClass());
			}
		}), 5.0f, false);
}

void AUSDFCharacterMeleeMonster::AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	AttackFinished();
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

			AUSDFCharacterBase* HitCharacter = Cast<AUSDFCharacterBase>(HitResult.GetActor());

			if (HitCharacter && bIsExist == false)
			{
				float DamageAmount = Stat->GetNormalMonsterStat().DefaultAttack;
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
					HitReactableCharacter->HitReact(DamageAmount, EHitReactType::Default ,this);
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 15.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
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

			AUSDFCharacterBase* HitCharacter = Cast<AUSDFCharacterBase>(HitResult.GetActor());

			if (HitCharacter && bIsExist == false)
			{
				float DamageAmount = Stat->GetNormalMonsterStat().StrongAttack;
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
					HitReactableCharacter->HitReact(DamageAmount, EHitReactType::Default ,this);
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = AttackBase + (AttackTip - AttackBase) * 0.5f;
	float CapsuleHalfHeight = (AttackTip - AttackBase).Length() * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, 15.0f, FRotationMatrix::MakeFromZ(AttackTip - AttackBase).ToQuat(), DrawColor, false, 5.0f);
#endif
}
