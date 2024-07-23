// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBossDarkMage.h"
#include "AI/USDFBossDarkMageAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameData/USDFGameSingleton.h"
#include "CharacterStat/USDFBossMonsterStatComponent.h"
#include "Projectiles/USDFEnemyProjectile.h"
#include "Projectiles/USDFDarkMageMeteoSpawner.h"
#include "Projectiles/USDFDarkMageUpLaserProjectile.h"
#include "Projectiles/USDFDarkMageElectLaserProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Animation/USDFBossDarkMageAnimInstance.h"
#include "AI/USDFAIController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationSystem.h"

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

	static ConstructorHelpers::FClassFinder<AUSDFDarkMageMeteoSpawner> MeteoSpawnerClassRef(TEXT("/Game/Blueprint/Projectiles/BP_USDFDarkMageMeteoSpawner.BP_USDFDarkMageMeteoSpawner_C"));
	if (MeteoSpawnerClassRef.Class)
	{
		MeteoSpawnerClass = MeteoSpawnerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AUSDFDarkMageUpLaserProjectile> UpLaserClassRef(TEXT("/Game/Blueprint/Projectiles/BP_USDFDarkMageUpLaserProjectile.BP_USDFDarkMageUpLaserProjectile_C"));
	if (UpLaserClassRef.Class)
	{
		UpLaserClass = UpLaserClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AUSDFDarkMageElectLaserProjectile> ElectLaserClassRef(TEXT("/Game/Blueprint/Projectiles/BP_USDFDarkMageElectLaser.BP_USDFDarkMageElectLaser_C"));
	if (ElectLaserClassRef.Class)
	{
		ElectLaserClass = ElectLaserClassRef.Class;
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeteoAttackMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_Meteo.BS_USDF_Boss_DarkMage_Meteo"));
	if (MeteoAttackMontageRef.Object)
	{
		AttackMontages.Add(EDarkMageAttackType::Meteo, MeteoAttackMontageRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UpLaserAttackMontageRef(TEXT("/Game/Animation/NonPlayer/BossDarkMage/BS_USDF_Boss_DarkMage_UpLaser.BS_USDF_Boss_DarkMage_UpLaser"));
	if (UpLaserAttackMontageRef.Object)
	{
		AttackMontages.Add(EDarkMageAttackType::UpLaser, UpLaserAttackMontageRef.Object);
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
		case EAIAttackType::Attack1:
		{
			CurrentAttackType = EDarkMageAttackType::Meteo;
		}
			break;
		case EAIAttackType::Attack2:
		{
			CurrentAttackType = EDarkMageAttackType::UpLaser;
		}
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

void AUSDFCharacterBossDarkMage::SpawnProjectile()
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
		case EDarkMageAttackType::Meteo:
		{
			AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
			if (AIController == nullptr)
				break;

			const AActor* Target = AIController->GetAttackTarget();

			if (Target == nullptr)
				break;

			UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
			if (Nav == nullptr)
				break;

			int32 DefaultCount = 3;
			int32 AddCount = static_cast<int32>((1.0f - Stat->GetCurrentHp() / Stat->GetMaxHp()) * 10.0f);

			int32 TotCount = DefaultCount + AddCount;
			for (int32 i = 0; i < TotCount; ++i)
			{
				float angle = 180.0f * (2*i+1) / (2 * TotCount );
				float radius = 800.0f;

				FVector RightVector = GetActorRightVector();
				FVector SpawnerVector = RightVector.RotateAngleAxis(angle,GetActorForwardVector());
				FVector SpawnerLocation = GetActorLocation() + SpawnerVector * radius;

				AUSDFDarkMageMeteoSpawner* MeteoSpawner = GetWorld()->SpawnActor<AUSDFDarkMageMeteoSpawner>(MeteoSpawnerClass, FTransform::Identity);
				if (MeteoSpawner)
				{
					MeteoSpawner->SetActorLocation(SpawnerLocation);
					MeteoSpawner->SetAttackDamage(Stat->GetBossMonsterStat().Skill1);
					
					FVector TargetLocation = Target->GetActorLocation();
					FNavLocation EndLocation = {};
					if (Nav->GetRandomPointInNavigableRadius(TargetLocation, 500.0f, EndLocation))
					{
						TargetLocation = EndLocation.Location;
					}

					MeteoSpawner->SetActorRotation(FRotationMatrix::MakeFromX(TargetLocation - SpawnerLocation).Rotator());
				}
			}
		}
			break;
		default:
			break;
	}
}

void AUSDFCharacterBossDarkMage::SpawnLaser(int32 InParam)
{
	AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
	if (AIController == nullptr)
		return;

	const AActor* Target = AIController->GetAttackTarget();

	if (Target == nullptr)
		return;

	UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (Nav == nullptr)
		return;

	switch (CurrentAttackType)
	{
		case EDarkMageAttackType::UpLaser:
		{
			switch (InParam)
			{
				case 0:
				{
					AUSDFDarkMageUpLaserProjectile* UpLaserProjectile = GetWorld()->SpawnActorDeferred<AUSDFDarkMageUpLaserProjectile>(UpLaserClass, FTransform::Identity);
					if (UpLaserProjectile)
					{
						UpLaserProjectile->SetAttackDamage(Stat->GetBossMonsterStat().Skill2);

						FVector TargetLocation = Target->GetActorLocation();
						FNavLocation EndLocation = {};
						if (Nav->GetRandomPointInNavigableRadius(TargetLocation, 300.0f, EndLocation))
						{
							TargetLocation = EndLocation.Location;
						}

						FTransform Transform = FTransform::Identity;
						Transform.SetLocation(TargetLocation);
						Transform.SetRotation(FRotationMatrix::Identity.ToQuat());

						UpLaserProjectile->FinishSpawning(Transform);
					}
				}
					break;
				case 1:
				{
					for (int32 i = 0; i < 2; ++i)
					{
						AUSDFDarkMageElectLaserProjectile* ElectLaserProjectile = GetWorld()->SpawnActorDeferred<AUSDFDarkMageElectLaserProjectile>(ElectLaserClass, FTransform::Identity);
						if (ElectLaserProjectile)
						{
							ElectLaserProjectile->SetAttackDamage(Stat->GetBossMonsterStat().Skill2);
							float angle = 180.0f * (2*i+1)/ 4.0f;
							float radius = 800.0f;

							FVector SpawnVector = GetActorRightVector().RotateAngleAxis(angle, GetActorForwardVector());
							FVector SpawnLocation = GetActorLocation() + SpawnVector * radius;

							FVector TargetForwardVector = (Target->GetActorLocation() - SpawnLocation).GetSafeNormal();
							float TargetDistance = (Target->GetActorLocation() - SpawnLocation).Length();
							FVector NewRightVector = TargetForwardVector.Cross(FVector::UpVector);
							FVector NewLookLocation = FVector(SpawnLocation.X, SpawnLocation.Y, Target->GetActorLocation().Z);
							TargetForwardVector = FVector::UpVector.Cross(NewRightVector);

							FVector ProjectileLookPosition = NewLookLocation + NewRightVector * TargetDistance * ((i == 0) ? 1 : -1) * (1.0f/FMath::Tan(PI / 3.0f));
							FVector ProjectileForwardVector = (ProjectileLookPosition - SpawnLocation).GetSafeNormal();
							FVector TempVector = (Target->GetActorLocation() - ProjectileLookPosition).GetSafeNormal();

							FTransform Transform = FTransform::Identity;
							Transform.SetLocation(SpawnLocation);
							Transform.SetRotation(FRotationMatrix::MakeFromXY(ProjectileForwardVector,TempVector).ToQuat());

							ElectLaserProjectile->FinishSpawning(Transform);
						}
					}
				}
					break;
				default:
					break;
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
