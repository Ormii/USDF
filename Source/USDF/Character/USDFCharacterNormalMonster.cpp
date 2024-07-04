// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterNormalMonster.h"
#include "UI/USDFWidgetComponent.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "Physics/USDFCollision.h"
#include "Animation/USDFMeleeMonsterAnimInstance.h"
#include "Animation/USDFNonPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/USDFAIController.h"

AUSDFCharacterNormalMonster::AUSDFCharacterNormalMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	// CDO
	HpBarWidget = CreateDefaultSubobject<UUSDFWidgetComponent>(TEXT("HpBarWidget"));
	HpBarWidget->SetupAttachment(GetMesh());
	HpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

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
		}

		GetCharacterMovement()->DisableMovement();
	}
	else
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	
}

bool AUSDFCharacterNormalMonster::GetHitReactState()
{
	return bHitReactState;
}

void AUSDFCharacterNormalMonster::HitReact(const FHitResult& HitResult, const float DamageAmount, const AActor* HitCauser)
{
	UUSDFMeleeMonsterAnimInstance* AnimInstance = Cast<UUSDFMeleeMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	if (HitCauser == nullptr)
		return;

	if (Stat->GetCurrentHp() <= 0.0f)
		return;

	AnimInstance->StopAllMontages(0.0f);

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
		AnimInstance->Montage_Play(HitReactMontage);
		bHitReactState = true;
		HitReactTime = 6.0f;
	}
}

void AUSDFCharacterNormalMonster::SetDead()
{
	Super::SetDead();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]() {
			Destroy();
		}), 5.0f, false);

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
