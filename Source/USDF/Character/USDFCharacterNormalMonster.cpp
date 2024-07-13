// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterNormalMonster.h"
#include "UI/USDFWidgetComponent.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "Physics/USDFCollision.h"
#include "Animation/USDFMeleeMonsterAnimInstance.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/USDFAIController.h"
#include "CharacterStat/USDFNormalMonsterStatComponent.h"
#include "GameData/USDFNormalMonsterStat.h"

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

	CurrentHitReactType = EHitReactType::None;
	UpperHitTime = 0;
}

void AUSDFCharacterNormalMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->OnHpZeroDelegate.AddUObject(this, &AUSDFCharacterNormalMonster::SetDead);
}

void AUSDFCharacterNormalMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bHitReactState)
	{
		HitReactTime -= DeltaSeconds;
		if (HitReactTime <= 0)
		{
			bHitReactState = false;
			HitReactTime = 0;
			CurrentHitReactType = EHitReactType::None;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
			
			UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
			if (NonPlayerAnimInstance)
			{
				NonPlayerAnimInstance->StopAllMontages(0.0f);
			}
		}

		switch (CurrentHitReactType)
		{
			case EHitReactType::Upper:
			{
				if (UpperHitTime > 0)
				{
					UpperHitTime -= DeltaSeconds;
					GetCharacterMovement()->Velocity.Z = FMath::FInterpTo(GetCharacterMovement()->Velocity.Z, 0, 10*DeltaSeconds, 1.0f);
				}
			}
				break;
			default:
				break;
		}
	}	
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

bool AUSDFCharacterNormalMonster::GetHitReactState()
{
	return bHitReactState;
}

void AUSDFCharacterNormalMonster::HitReact(const float DamageAmount, EHitReactType HitReactType, const AActor* HitCauser)
{
	UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (NonPlayerAnimInstance == nullptr)
		return;

	if (HitCauser == nullptr)
		return;

	if (Stat->GetCurrentHp() <= 0.0f)
		return;

	NonPlayerAnimInstance->StopAllMontages(0.0f);
	HitCharaters.Empty();

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
	else if(ForwardDotProduct <= -0.5f)
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
		bHitReactState = true;
		HitReactTime = 3.0f;

		CurrentHitReactType = HitReactType;
		switch (HitReactType)
		{
			case EHitReactType::Upper:
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				GetCharacterMovement()->Velocity += FVector(0.0f, 0.0f, 2000.0f);
				UE_LOG(LogTemp, Display, TEXT("Velocity x:%f y:%f, z:%f"), GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, GetCharacterMovement()->Velocity.Z);
				UpperHitTime = 0.1f;
			}
				break;
			default:
			{
				GetCharacterMovement()->DisableMovement();
				GetCharacterMovement()->StopActiveMovement();
				GetCharacterMovement()->StopMovementImmediately();
			}
				break;
		}
	}
}

void AUSDFCharacterNormalMonster::SetDead()
{
	Super::SetDead();

	UUSDFNonPlayerAnimInstance* NonPlayerAnimInstance = Cast<UUSDFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (NonPlayerAnimInstance && DeadAnimMontage)
	{
		bHitReactState = false;
		HitReactTime = 0.0f;
		NonPlayerAnimInstance->StopAllMontages(0.0f);
		NonPlayerAnimInstance->Montage_Play(DeadAnimMontage);
	}

	AUSDFAIController* AIController = Cast<AUSDFAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}
	

	HpBarWidget->SetHiddenInGame(true);
}

void AUSDFCharacterNormalMonster::SetupHpBarWidget(UUSDFUserWidget* InUserWidget)
{
	UUSDFEnemyHpBarWidget* HpBar = Cast<UUSDFEnemyHpBarWidget>(InUserWidget);
	Stat->OnHpChangedDelegate.AddUObject(HpBar, &UUSDFEnemyHpBarWidget::UpdateHpBar);

	HpBar->SetMaxHp(Stat->GetMaxHp());
	HpBar->UpdateHpBar(Stat->GetCurrentHp());
}

float AUSDFCharacterNormalMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	return 0.0f;
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
