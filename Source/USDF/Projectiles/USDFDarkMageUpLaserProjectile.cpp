// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFDarkMageUpLaserProjectile.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AUSDFDarkMageUpLaserProjectile::AUSDFDarkMageUpLaserProjectile()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BaseEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Laser/NS_MeshLaser_DarkMagic.NS_MeshLaser_DarkMagic"));
	if (BaseEffectRef.Object)
	{
		BaseEffect->SetAsset(BaseEffectRef.Object);
	}

	BoxCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 755.0f));
	BoxCollision->SetBoxExtent(FVector(85.0f, 85.0f, 760.0f));
}

void AUSDFDarkMageUpLaserProjectile::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->SetGenerateOverlapEvents(false);
	BoxCollision->SetVisibility(false);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		BoxCollision->SetVisibility(true);
		BoxCollisionActivate();
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.1f, false);
}

void AUSDFDarkMageUpLaserProjectile::BoxCollisionActivate()
{
	BoxCollision->SetGenerateOverlapEvents(true);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		BoxCollisionDeActivate();
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.75f, false);
}

void AUSDFDarkMageUpLaserProjectile::BoxCollisionDeActivate()
{
	BoxCollision->SetVisibility(false);
	BoxCollision->SetGenerateOverlapEvents(false);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		Destroy();
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.4f, false);
}
