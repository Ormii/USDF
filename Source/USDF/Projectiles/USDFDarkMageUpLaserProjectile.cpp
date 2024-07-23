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

	bBoxCollisionActivate = false;
	ActivateTime = 0.0f;
}

void AUSDFDarkMageUpLaserProjectile::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->SetGenerateOverlapEvents(false);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		BoxCollisionActivate();
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.1f, false);
}

void AUSDFDarkMageUpLaserProjectile::BeginDestroy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::BeginDestroy();
}

void AUSDFDarkMageUpLaserProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBoxCollisionActivate)
	{
		ActivateTime += DeltaTime;
		if (ActivateTime >= 0.75f)
		{
			bBoxCollisionActivate = false;
			ActivateTime = 0.0f;
			BoxCollision->SetGenerateOverlapEvents(false);

			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]() {
				Destroy();
			});

			if (GetWorld() != nullptr)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.4f, false);
			}
		}
	}
}

void AUSDFDarkMageUpLaserProjectile::BoxCollisionActivate()
{
	bBoxCollisionActivate = true;
	BoxCollision->SetGenerateOverlapEvents(true);
}
