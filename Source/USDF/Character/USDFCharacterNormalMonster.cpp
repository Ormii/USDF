// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterNormalMonster.h"
#include "UI/USDFWidgetComponent.h"
#include "UI/USDFEnemyHpBarWidget.h"
#include "Physics/USDFCollision.h"
#include "Animation/USDFNonPlayerAnimInstance.h"

AUSDFCharacterNormalMonster::AUSDFCharacterNormalMonster()
{
	// CDO
	HpBarWidget = CreateDefaultSubobject<UUSDFWidgetComponent>(TEXT("HpBarWidget"));
	HpBarWidget->SetupAttachment(GetMesh());
	HpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(CPROFILE_USDFCHARACTERMESH);

	static ConstructorHelpers::FClassFinder<UUSDFEnemyHpBarWidget> EnemyHpBarWidgetClassRef(TEXT("/Game/Blueprint/Widget/WBP_USDFEnemyHpBarWidget.WBP_USDFEnemyHpBarWidget_C"));
	if (EnemyHpBarWidgetClassRef.Class)
	{
		HpBarWidget->SetWidgetClass(EnemyHpBarWidgetClassRef.Class);
		HpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarWidget->SetDrawSize(FVector2D(200.0f, 15.0f));
		HpBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		NonPlayerAnimInstance->StopAllMontages(0.0f);
		NonPlayerAnimInstance->Montage_Play(DeadAnimMontage);
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
