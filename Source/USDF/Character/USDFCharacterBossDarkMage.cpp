// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBossDarkMage.h"
#include "AI/USDFBossDarkMageAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "CharacterStat/USDFBossMonsterStatComponent.h"
#include "Projectiles/USDFEnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Animation/USDFBossDarkMageAnimInstance.h"
#include "AI/USDFAIController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	static ConstructorHelpers::FClassFinder<AUSDFEnemyProjectile> DefaultAtkProjectileClassRef(TEXT("/Game/Blueprint/Projectiles/BP_USDFDarkMageDefaultProjectile.BP_USDFDarkMageDefaultProjectile_C"));
	if (DefaultAtkProjectileClassRef.Class)
	{
		DefaultAtkProjectileClass = DefaultAtkProjectileClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactFrontMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_HitReact_Front.BS_USDF_Boss_DarkMage_HitReact_Front"));
	if (HitReactFrontMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Front, HitReactFrontMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactBackMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_HitReact_Back.BS_USDF_Boss_DarkMage_HitReact_Back"));
	if (HitReactBackMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Back, HitReactBackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactRightMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_HitReact_Right.BS_USDF_Boss_DarkMage_HitReact_Right"));
	if (HitReactRightMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Right, HitReactRightMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactLeftMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_HitReact_Left.BS_USDF_Boss_DarkMage_HitReact_Left"));
	if (HitReactLeftMontageRef.Object)
	{
		HitReactAnimMontage.Add(EHitReactDirection::Left, HitReactLeftMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultAttackMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_DefaultAtk.BS_USDF_Boss_DarkMage_DefaultAtk"));
	if (DefaultAttackMontageRef.Object)
	{
		AttackMontages.Add(EDarkMageAttackType::DefaultAttack, DefaultAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TeleportEffectRef(TEXT("/Game/ReferenceAsset/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Vampire.NS_Dash_Vampire"));
	if (TeleportEffectRef.Object)
	{
		TeleportEffect = TeleportEffectRef.Object;
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
	switch (InAIAttackType)
	{
	case EAIAttackType::Range:
	{
		CurrentAttackType = EDarkMageAttackType::DefaultAttack;
	}
	break;
	case EAIAttackType::Melee:
	case EAIAttackType::Dash:
		break;
	default:
		break;
	}

	if (CurrentAttackType != EDarkMageAttackType::None)
	{
		UUSDFBossDarkMageAnimInstance* AnimInstance = Cast<UUSDFBossDarkMageAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			UAnimMontage* PlayAttackMontage = AttackMontages[CurrentAttackType];

			AnimInstance->StopAllMontages(0.0f);
			AnimInstance->Montage_Play(PlayAttackMontage);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &AUSDFCharacterBossDarkMage::AttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(OnMontageEnded, PlayAttackMontage);
		}
	}
}

void AUSDFCharacterBossDarkMage::AttackFinished()
{
	Super::AttackFinished();
}

void AUSDFCharacterBossDarkMage::AttackMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	AttackFinished();
	CurrentAttackType = EDarkMageAttackType::None;
}

void AUSDFCharacterBossDarkMage::AttackFire()
{
	if (DefaultAtkProjectile == nullptr)
		return;
	
	AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
	if (AIController == nullptr)
		return;

	const AActor* Target = AIController->GetAttackTarget();
	if (Target == nullptr)
		return;

	DefaultAtkProjectile->DetachAllSceneComponents(GetMesh(), FDetachmentTransformRules::KeepWorldTransform);
	DefaultAtkProjectile->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DefaultAtkProjectile->GetProjectileMovementComp()->bSimulationEnabled = true;
	DefaultAtkProjectile->GetProjectileMovementComp()->ProjectileGravityScale = 0.0f;

	DefaultAtkProjectile->SetAttackDamage(Stat->GetBossMonsterStat().DefaultAttack);

	FVector StartLocation = DefaultAtkProjectile->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector ForwardVector = (TargetLocation - StartLocation).GetSafeNormal();
	DefaultAtkProjectile->SetActorRotation(FRotationMatrix::MakeFromX(ForwardVector).ToQuat());
	DefaultAtkProjectile->GetProjectileMovementComp()->Velocity = ForwardVector * 3000.0f;
}

void AUSDFCharacterBossDarkMage::SpawnOrb()
{
	switch (CurrentAttackType)
	{
		case EDarkMageAttackType::DefaultAttack:
		{
			DefaultAtkProjectile = GetWorld()->SpawnActor<AUSDFEnemyProjectile>(DefaultAtkProjectileClass, FTransform::Identity);
			if (DefaultAtkProjectile != nullptr)
			{
				DefaultAtkProjectile->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "default_orb_spawn_socket");
				DefaultAtkProjectile->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				DefaultAtkProjectile->GetProjectileMovementComp()->bSimulationEnabled = false;
			}
		}
			break;
		default:
			break;
	}
}

void AUSDFCharacterBossDarkMage::TeleportStart()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	SaveMaxFlySpeed = GetCharacterMovement()->MaxFlySpeed;
	SaveMaxAcceleration = GetCharacterMovement()->MaxAcceleration;

	GetCharacterMovement()->MaxFlySpeed = 5000.0f;
	GetCharacterMovement()->MaxAcceleration = 99999.0f;


	GetMesh()->SetVisibility(false, true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TeleportEffect, GetActorLocation(), FRotationMatrix::Identity.Rotator());;
	if (pNiagaraCompo != nullptr)
	{
		pNiagaraCompo->Activate();
		pNiagaraCompo->SetAutoDestroy(true);
	}
}

void AUSDFCharacterBossDarkMage::TeleportEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	

	GetCharacterMovement()->MaxFlySpeed = SaveMaxFlySpeed;
	GetCharacterMovement()->MaxAcceleration = SaveMaxAcceleration;

	GetMesh()->SetVisibility(true, true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TeleportEffect, GetActorLocation(), FRotationMatrix::Identity.Rotator());
	if (pNiagaraCompo != nullptr)
	{
		pNiagaraCompo->Activate();
		pNiagaraCompo->SetAutoDestroy(true);
	}
}


void AUSDFCharacterBossDarkMage::OnDeath()
{
}
