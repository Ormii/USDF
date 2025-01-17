// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterRangeMonster.h"
#include "AI/USDFRangeMonsterAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "GameData/USDFNormalMonsterStat.h"
#include "CharacterStat/USDFNormalMonsterStatComponent.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "UI/USDFWidgetComponent.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "Physics/USDFCollision.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"
#include "Character/USDFCharacterPlayer.h"


AUSDFCharacterRangeMonster::AUSDFCharacterRangeMonster()
{
	//AI
	AIControllerClass = AUSDFRangeMonsterAIController::StaticClass();
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

	// Mesh
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -105.0f));

	HpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/Biochemical_Monster_1/baze_mech/SK_baze_mesh1.SK_baze_mesh1"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/NonPlayer/ABP_USDF_RangeMonsterAnimInstance.ABP_USDF_RangeMonsterAnimInstance_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadAnimMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/AM_USDF_RangeMonster_DeadAction.AM_USDF_RangeMonster_DeadAction"));
	if (DeadAnimMontageRef.Object)
	{
		DeadAnimMontage = DeadAnimMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactFrontMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/BS_USDF_RangeMonster_HitReact_Front.BS_USDF_RangeMonster_HitReact_Front"));
	if (HitReactFrontMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Front, HitReactFrontMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactBackMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/BS_USDF_RangeMonster_HitReact_Back.BS_USDF_RangeMonster_HitReact_Back"));
	if (HitReactBackMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Back, HitReactBackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactRightMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/BS_USDF_RangeMonster_HitReact_Right.BS_USDF_RangeMonster_HitReact_Right"));
	if (HitReactRightMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Right, HitReactRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactLeftMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/BS_USDF_RangeMonster_HitReact_Left.BS_USDF_RangeMonster_HitReact_Left"));
	if (HitReactLeftMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Left, HitReactLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeakAttackMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/AM_USDF_RangeMonster_WeakAttack.AM_USDF_RangeMonster_WeakAttack"));
	if (WeakAttackMontageRef.Object)
	{
		ActionMontages.Add(ERangeMonsterActionType::WeakAttack, WeakAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StrongAttackMontageRef(TEXT("/Game/Animation/NonPlayer/RangeMonster/AM_USDF_RangeMonster_StrongAttack.AM_USDF_RangeMonster_StrongAttack"));
	if (StrongAttackMontageRef.Object)
	{
		ActionMontages.Add(ERangeMonsterActionType::StrongAttack, StrongAttackMontageRef.Object);
	}

	for (int i = 0; i < 3; ++i)
	{
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackHitEffectRef(*FString::Printf(TEXT("/Game/ReferenceAsset/RealisticBlood/Slash/Niagara/NS_Slash_%d.NS_Slash_%d"), i, i));
		if (AttackHitEffectRef.Object)
		{
			AttackHitEffects.Add(AttackHitEffectRef.Object);
		}
	}
}

void AUSDFCharacterRangeMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUSDFGameSingleton* GameSingleton = Cast<UUSDFGameSingleton>(GEngine->GameSingleton.Get());

	Stat->InitNormalMonsterStat(GameSingleton->GetNormalMonsterStat("RangeMonster"));
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetNormalMonsterStat().RunSpeed;
}

void AUSDFCharacterRangeMonster::ActionByAI(EAIActionType InAIAttackType)
{
	switch (InAIAttackType)
	{
		case EAIActionType::Range:
		{
			int32 RandValue = FMath::RandRange(0, 100);
			if (RandValue >= 80)
				CurrentActionType = ERangeMonsterActionType::StrongAttack;
			else
				CurrentActionType = ERangeMonsterActionType::WeakAttack;
		}
			break;
		default:
			break;
	}

	if (CurrentActionType != ERangeMonsterActionType::None)
	{
		UUSDFNonPlayerAnimInstance* AnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			UAnimMontage* PlayAttackMontage = ActionMontages[CurrentActionType];

			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(PlayAttackMontage);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &AUSDFCharacterRangeMonster::ActionMontageEnded);
			AnimInstance->Montage_SetEndDelegate(OnMontageEnded, PlayAttackMontage);
		}
	}
}

void AUSDFCharacterRangeMonster::ActionFinished()
{
	Super::ActionFinished();
}

void AUSDFCharacterRangeMonster::ActionMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ActionFinished();
	CurrentActionType = ERangeMonsterActionType::None;
}

void AUSDFCharacterRangeMonster::AttackFire()
{
	AUSDFRangeMonsterAIController* AIController = Cast<AUSDFRangeMonsterAIController>(GetController());
	const AActor* Target = (AIController != nullptr) ? AIController->GetAttackTarget() : nullptr;
	FVector StartLocation = GetMesh()->GetSocketLocation("rifle_fire_socket");
	FVector EndLocation = (Target != nullptr) ? Target->GetActorLocation() : StartLocation + GetActorForwardVector() * Stat->GetNormalMonsterStat().AttackRange;

	FHitResult HitResult{};
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Fire), false, this);
	bool bHitted = GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation, EndLocation, ECC_Visibility, Params);
	bool bCharacterHit = false;
	if (bHitted)
	{
		AUSDFCharacterPlayer* HitCharacter = Cast<AUSDFCharacterPlayer>(HitResult.GetActor());
		if (HitCharacter == nullptr)
			return;

		IUSDFDamageableInterface* DamageableTarget = Cast<IUSDFDamageableInterface>(HitCharacter);

		if (DamageableTarget)
		{
			float DamageAmount = Stat->GetNormalMonsterStat().DefaultAttack;

			FDamageInfo DamageInfo = {};
			DamageInfo.DamageAmount = DamageAmount;
			DamageInfo.DamageCauser = this;
			DamageInfo.DamageType = EDamageType::HitDefault;

			DamageableTarget->TakeDamage(DamageInfo);

			bCharacterHit = true;
			int32 AttackHitEffectIndex = FMath::RandRange(0, 2);
			if (AttackHitEffects[AttackHitEffectIndex] != nullptr)
			{
				FVector BoneLocation = FVector::ZeroVector;
				HitCharacter->GetMesh()->FindClosestBone(HitResult.Location, &BoneLocation);
				FVector ImpactNormal = (HitResult.Location - BoneLocation).GetSafeNormal();

				UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackHitEffects[AttackHitEffectIndex], BoneLocation, FRotationMatrix::MakeFromZ(ImpactNormal).Rotator());
				if (pNiagaraCompo != nullptr)
				{
					pNiagaraCompo->Activate();
				}
			}
		}
	}


#if ENABLE_DRAW_DEBUG
	///FColor Color = (bCharacterHit) ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, 0.2f);

	//if (bCharacterHit)
	//{
		//DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, Color, false, 0.2f);
	//}
#endif
}

void AUSDFCharacterRangeMonster::OnDeath()
{
	Super::OnDeath();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		Destroy();
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 5.0f, false);
}

