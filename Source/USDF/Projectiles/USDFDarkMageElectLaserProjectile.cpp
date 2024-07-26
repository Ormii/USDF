// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFDarkMageElectLaserProjectile.h"
#include "Components/BoxComponent.h"
#include "Physics/USDFCollision.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Interface/USDFDamageableInterface.h"

AUSDFDarkMageElectLaserProjectile::AUSDFDarkMageElectLaserProjectile()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BaseEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Laser/NS_Laser_DarkMagic.NS_Laser_DarkMagic"));
	if (BaseEffectRef.Object)
	{
		BaseEffect->SetAsset(BaseEffectRef.Object);
	}
	
	Distance = 10000.0f;
	bRotate = false;
	AcculmulateAngle = 0.1f;
}

void AUSDFDarkMageElectLaserProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		bRotate = true;
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
}

void AUSDFDarkMageElectLaserProjectile::BeginDestroy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::BeginDestroy();
}

void AUSDFDarkMageElectLaserProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FVector UpVector = GetActorUpVector();
		AcculmulateAngle += AcculmulateAngle * DeltaTime;
		Mesh->AddLocalRotation(FRotator(0.0f, AcculmulateAngle, 0.0f));
	}

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ElectLaser), false, this);
	FHitResult HitResult;

		
	float Radius = 35.0f;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorForwardVector() * Distance;

	bool bHitted = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity,CCHANNEL_USDF_NON_PLAYER_CHARACTER, FCollisionShape::MakeSphere(Radius),Params);
	if (bHitted)
	{
		EndLocation = HitResult.Location;
		IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(HitResult.GetActor());
		if (Damageable)
		{
			if (HitCharacters.Find(HitResult.GetActor()) == nullptr)
			{
				FDamageInfo DamageInfo{};
				DamageInfo.DamageAmount = AttackDamage;
				DamageInfo.DamageCauser = GetOwner();
				DamageInfo.DamageType = EDamageType::HitDefault;
				Damageable->TakeDamage(DamageInfo);

				HitCharacters.Add(HitResult.GetActor());
			}
		}
	}
	else
	{
		Destroy();
	}

	BaseEffect->SetNiagaraVariableVec3("Beam End", EndLocation);

#if ENABLE_DRAW_DEBUG

	/*
	FVector Position = (StartLocation + EndLocation) * 0.5f;
	float Length = (EndLocation - StartLocation).Length() * 0.5f;

	FColor Color = (bHitted == true) ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), Position, Length, Radius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), Color, false, 0.1f);
	*/
#endif

}
