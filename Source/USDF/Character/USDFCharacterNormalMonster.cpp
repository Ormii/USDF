// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterNormalMonster.h"
#include "UI/USDFWidgetComponent.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "Animation/USDFMeleeMonsterAnimInstance.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/USDFNormalMonsterStatComponent.h"
#include "Physics/USDFCollision.h"
#include "GameData/USDFNormalMonsterStat.h"
#include "AI/USDFAIController.h"
#include "Perception/AISense_Damage.h"
#include "Damage/USDFDamageSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/USDFPatrolRoute.h"

AUSDFCharacterNormalMonster::AUSDFCharacterNormalMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	

	// CDO
	HpBarWidget = CreateDefaultSubobject<UUSDFWidgetComponent>(TEXT("HpBarWidget"));
	HpBarWidget->SetupAttachment(GetMesh());
	HpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	Stat = CreateDefaultSubobject<UUSDFNormalMonsterStatComponent>(TEXT("Stat"));

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UUSDFEnemyHpBarWidget> EnemyHpBarWidgetClassRef(TEXT("/Game/Blueprint/Widget/WBP_USDFEnemyHpBarWidget.WBP_USDFEnemyHpBarWidget_C"));
	if (EnemyHpBarWidgetClassRef.Class)
	{
		HpBarWidget->SetWidgetClass(EnemyHpBarWidgetClassRef.Class);
		HpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarWidget->SetDrawSize(FVector2D(200.0f, 15.0f));
		HpBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AUSDFCharacterNormalMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DamageSystem->OnDeath.BindUObject(this, &AUSDFCharacterNormalMonster::OnDeath);
	DamageSystem->OnDamageResponse.BindUObject(this, &AUSDFCharacterNormalMonster::OnDamageResponse);
}

void AUSDFCharacterNormalMonster::BeginPlay()
{
	Super::BeginPlay();

	AActor* Route = UGameplayStatics::GetActorOfClass(this, AUSDFPatrolRoute::StaticClass());
	if (Route != nullptr)
	{
		PatrolRoute = Cast<AUSDFPatrolRoute>(Route);
	}
	
}

void AUSDFCharacterNormalMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

float AUSDFCharacterNormalMonster::GetAIPatrolRadius()
{
	return Stat->GetNormalMonsterStat().PatrolRadius;  
}

float AUSDFCharacterNormalMonster::GetAIDetectRadius()
{
	return Stat->GetNormalMonsterStat().DetectRadius;
}

float AUSDFCharacterNormalMonster::GetAIAttackRange()
{
	return Stat->GetNormalMonsterStat().AttackRange;
}

float AUSDFCharacterNormalMonster::GetAITurnRateSpeed()
{
	return Stat->GetNormalMonsterStat().TurnRateSpeed;
}

AUSDFPatrolRoute* AUSDFCharacterNormalMonster::GetPatrolRoute()
{
	return PatrolRoute;
}

float AUSDFCharacterNormalMonster::GetMaxHealth()
{
	return Stat->GetMaxHp();
}

float AUSDFCharacterNormalMonster::GetCurrentHealth()
{
	return Stat->GetCurrentHp();
}

void AUSDFCharacterNormalMonster::Heal(float HealAmount)
{
	DamageSystem->Heal(Stat, HealAmount);
}

void AUSDFCharacterNormalMonster::TakeDamage(FDamageInfo DamageInfo)
{
	DamageSystem->TakeDamage(Stat, DamageInfo);
}

bool AUSDFCharacterNormalMonster::IsDead()
{
	return DamageSystem->GetIsDead();
}

float AUSDFCharacterNormalMonster::GetAIEQSTargetRadius()
{
	return Stat->GetNormalMonsterStat().EQSTargetRange;
}

float AUSDFCharacterNormalMonster::GetAIEQSDefendRadius()
{
	return Stat->GetNormalMonsterStat().EQSDefendRange;
}

void AUSDFCharacterNormalMonster::SetupHpBarWidget(UUSDFUserWidget* InUserWidget)
{
	UUSDFEnemyHpBarWidget* HpBar = Cast<UUSDFEnemyHpBarWidget>(InUserWidget);
	Stat->OnHpChangedDelegate.AddUObject(HpBar, &UUSDFEnemyHpBarWidget::UpdateHpBar);

	HpBar->SetMaxHp(Stat->GetMaxHp());
	HpBar->UpdateHpBar(Stat->GetCurrentHp());
}

float AUSDFCharacterNormalMonster::GetMaxWalkSpeed()
{
	return Stat->GetNormalMonsterStat().WalkSpeed;
}

float AUSDFCharacterNormalMonster::GetMaxRunSpeed()
{
	return Stat->GetNormalMonsterStat().RunSpeed;
}

void AUSDFCharacterNormalMonster::SetLocomotionState(ELocomotionState NewLocomotionState)
{
	Super::SetLocomotionState(NewLocomotionState);

	switch (NewLocomotionState)
	{
		case ELocomotionState::Idle:
		case ELocomotionState::Walk:
			GetCharacterMovement()->MaxWalkSpeed = Stat->GetNormalMonsterStat().WalkSpeed;
			break;
		case ELocomotionState::Run:
			GetCharacterMovement()->MaxWalkSpeed = Stat->GetNormalMonsterStat().RunSpeed;
			break;
		default:
			break;
	}
}

void AUSDFCharacterNormalMonster::OnHitReactMontageBlendOut(UAnimMontage* TargetMontage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
		if (AIController)
		{
			FAISensedParam Param = {};
			Param.bUseLastAttackTarget = true;
			AIController->SetCurrentAIState(EAIState::Attacking, Param);
		}
	}
}

void AUSDFCharacterNormalMonster::OnDeath()
{
	Super::OnDeath();

	UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (NonPlayerAnimInstance && DeadAnimMontage)
	{
		NonPlayerAnimInstance->StopAllMontages(0.0f);
		NonPlayerAnimInstance->Montage_Play(DeadAnimMontage);
	}

	AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
	if (AIController)
	{
		AIController->SetCurrentAIState(EAIState::Dead, FAISensedParam{});
		AIController->StopAI();
		AIController->UnPossess();
	}


	HpBarWidget->SetHiddenInGame(true);
}

void AUSDFCharacterNormalMonster::OnDamageResponse(FDamageInfo DamageInfo)
{
	Super::OnDamageResponse(DamageInfo);

	AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
	if (AIController == nullptr)
		return;

	FAISensedParam Param = {};
	Param.DamageInfo = DamageInfo;

	AIController->SetCurrentAIState(EAIState::Frozen, Param);

	AActor* HitCauser = DamageInfo.DamageCauser;

	UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (NonPlayerAnimInstance == nullptr)
		return;

	if (HitCauser == nullptr)
		return;

	if (Stat->GetCurrentHp() <= 0.0f)
		return;

	NonPlayerAnimInstance->StopAllMontages(0.0f);

	FVector ActorForwardVector = GetActorForwardVector();
	FVector ActorRightVector = GetActorRightVector();
	FVector HitActorForwardVector = HitCauser->GetActorForwardVector();
	
	float ForwardDotProduct = ActorForwardVector.Dot(HitActorForwardVector);
	float RightDotProduct = ActorRightVector.Dot(HitActorForwardVector);

	UAnimMontage* HitReactMontage = nullptr;
	if (ForwardDotProduct >= 0.5f)
	{
		HitReactMontage = HitReactAnimMontage[EHitReactDirection::Front];
	}
	else if (ForwardDotProduct <= -0.5f)
	{
		HitReactMontage = HitReactAnimMontage[EHitReactDirection::Back];
	}
	else if (RightDotProduct >= 0.0f)
	{
		HitReactMontage = HitReactAnimMontage[EHitReactDirection::Right];
	}
	else
	{
		HitReactMontage = HitReactAnimMontage[EHitReactDirection::Left];
	}

	if (HitReactMontage)
	{
		UE_LOG(LogTemp, Display, TEXT("Hit React"));

		NonPlayerAnimInstance->Montage_Play(HitReactMontage);
		AIController->StopMovement();

		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &AUSDFCharacterNormalMonster::OnHitReactMontageBlendOut);

		NonPlayerAnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, HitReactMontage);
	}
}
