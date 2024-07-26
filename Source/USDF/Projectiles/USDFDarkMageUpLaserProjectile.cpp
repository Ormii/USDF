// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFDarkMageUpLaserProjectile.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Physics/USDFCollision.h"
#include "Interface/USDFDamageableInterface.h"

AUSDFDarkMageUpLaserProjectile::AUSDFDarkMageUpLaserProjectile()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BaseEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Laser/NS_MeshLaser_DarkMagic.NS_MeshLaser_DarkMagic"));
	if (BaseEffectRef.Object)
	{
		BaseEffect->SetAsset(BaseEffectRef.Object);
	}


	bCollisionActivate = false;
	ActivateTime = 0.0f;
	LaserLength = 760.0f;
}

void AUSDFDarkMageUpLaserProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		CollisionActivate();
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

	if (bCollisionActivate)
	{
		ActivateTime += DeltaTime;
		if (ActivateTime >= 0.75f)
		{
			bCollisionActivate = false;
			ActivateTime = 0.0f;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]() {
				Destroy();
			});

			if (GetWorld() != nullptr)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.4f, false);
			}
		}

		FCollisionQueryParams Params(SCENE_QUERY_STAT(LaserHit), false, this);
		FHitResult HitResult;
		FVector StartPoint = GetActorLocation();
		FVector EndPoint = StartPoint + GetActorUpVector() * LaserLength;
		float Radius = 120.0f;
		bool bHitted = GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, CCHANNEL_USDF_NONPLAYERACTION, FCollisionShape::MakeSphere(40.0f), Params);
		
		if (bHitted)
		{
			if (HitCharacters.Find(HitResult.GetActor()) != nullptr)
				return;

			IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(HitResult.GetActor());
			if (Damageable == nullptr)
				return;

			FDamageInfo DamageInfo{};
			DamageInfo.DamageAmount = AttackDamage;
			DamageInfo.DamageCauser = GetOwner();
			DamageInfo.DamageType = EDamageType::HitDefault;

			Damageable->TakeDamage(DamageInfo);

			HitCharacters.Add(HitResult.GetActor());
		}

#if ENABLE_DRAW_DEBUG
		/*
			FColor Color = (bHitted == true) ? FColor::Green : FColor::Red;
			DrawDebugCapsule(GetWorld(), (StartPoint + EndPoint) / 2, LaserLength * 0.5, Radius, FQuat::Identity, Color, false, 0.1f);
		*/
#endif
	}
}

void AUSDFDarkMageUpLaserProjectile::CollisionActivate()
{
	bCollisionActivate = true;
}
