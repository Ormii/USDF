// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBossMonster.h"
#include "AI/USDFAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/USDFBossMonsterStatComponent.h"
#include "Damage/USDFDamageSystemComponent.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "Damage/USDFDamageSystemComponent.h"
#include "UI/USDFEnemyHpBarWidget.h"

AUSDFCharacterBossMonster::AUSDFCharacterBossMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	Stat = CreateDefaultSubobject<UUSDFBossMonsterStatComponent>(TEXT("Stat"));

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void AUSDFCharacterBossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	DamageSystem->OnDeath.BindUObject(this, &AUSDFCharacterBossMonster::OnDeath);
	DamageSystem->OnDamageResponse.BindUObject(this, &AUSDFCharacterBossMonster::OnDamageResponse);
}

void AUSDFCharacterBossMonster::BeginPlay()
{
	Super::BeginPlay();

}

void AUSDFCharacterBossMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

float AUSDFCharacterBossMonster::GetMaxWalkSpeed()
{
	return Stat->GetBossMonsterStat().WalkSpeed;
}

float AUSDFCharacterBossMonster::GetMaxRunSpeed()
{
	return Stat->GetBossMonsterStat().RunSpeed;
}

void AUSDFCharacterBossMonster::SetLocomotionState(ELocomotionState NewLocomotionState)
{
	Super::SetLocomotionState(NewLocomotionState);

	switch (NewLocomotionState)
	{
	case ELocomotionState::Idle:
	case ELocomotionState::Walk:
		GetCharacterMovement()->MaxWalkSpeed = Stat->GetBossMonsterStat().WalkSpeed;
		break;
	case ELocomotionState::Run:
		GetCharacterMovement()->MaxWalkSpeed = Stat->GetBossMonsterStat().RunSpeed;
		break;
	default:
		break;
	}
}

float AUSDFCharacterBossMonster::GetAIAttackRange()
{
	return Stat->GetBossMonsterStat().AttackRange;
}

float AUSDFCharacterBossMonster::GetAITurnRateSpeed()
{
	return Stat->GetBossMonsterStat().TurnRateSpeed;
}

float AUSDFCharacterBossMonster::GetAIEQSDefendRadius()
{
	return Stat->GetBossMonsterStat().EQSDefendRange;
}

void AUSDFCharacterBossMonster::OnHitReactMontageBlendOut(UAnimMontage* TargetMontage, bool bInterrupted)
{
}

float AUSDFCharacterBossMonster::GetCurrentHealth()
{
	return Stat->GetCurrentHp();
}

float AUSDFCharacterBossMonster::GetMaxHealth()
{
	return Stat->GetMaxHp();
}

void AUSDFCharacterBossMonster::Heal(float HealAmount)
{
	DamageSystem->Heal(Stat, HealAmount);
}

void AUSDFCharacterBossMonster::TakeDamage(FDamageInfo DamageInfo)
{
	DamageSystem->TakeDamage(Stat, DamageInfo);
}

bool AUSDFCharacterBossMonster::IsDead()
{
	return DamageSystem->GetIsDead();
}

void AUSDFCharacterBossMonster::OnDeath()
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
}

void AUSDFCharacterBossMonster::OnDamageResponse(FDamageInfo DamageInfo)
{

}

void AUSDFCharacterBossMonster::SetupHpBarWidget(UUSDFUserWidget* InUserWidget)
{
	UUSDFEnemyHpBarWidget* HpBar = Cast<UUSDFEnemyHpBarWidget>(InUserWidget);
	Stat->OnHpChangedDelegate.Clear();
	Stat->OnHpChangedDelegate.AddUObject(HpBar, &UUSDFEnemyHpBarWidget::UpdateHpBar);

	HpBar->SetMaxHp(Stat->GetMaxHp());
	HpBar->UpdateHpBar(Stat->GetCurrentHp());
}
