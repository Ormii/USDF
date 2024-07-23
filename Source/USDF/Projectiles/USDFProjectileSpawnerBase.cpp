// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFProjectileSpawnerBase.h"
#include "Components/ArrowComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Projectiles/USDFProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AUSDFProjectileSpawnerBase::AUSDFProjectileSpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	SpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPosition"));
	RootComponent = SpawnPosition;


	SpawnEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnEffect"));
	SpawnEffect->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AUSDFProjectileSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate{};
	TimerDelegate.BindUObject(this, &AUSDFProjectileSpawnerBase::SpawnProjectile);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SpawnDelayTime, false);

	SpawnEffect->Activate();
}

// Called every frame
void AUSDFProjectileSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUSDFProjectileSpawnerBase::SpawnProjectile()
{
	AUSDFProjectileBase* Projectile = GetWorld()->SpawnActor<AUSDFProjectileBase>(ProjectileClass,FTransform::Identity);
	if (Projectile)
	{
		Projectile->SetActorLocation(GetActorLocation());
		Projectile->SetActorRotation(GetActorRotation());

		Projectile->GetProjectileMovementComp()->Velocity = GetActorForwardVector() * ProjectileInitSpeed;
		Projectile->GetProjectileMovementComp()->ProjectileGravityScale = 0.0f;

		Projectile->SetAttackDamage(AttackDamage);

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate{};
		
		TimerDelegate.BindLambda([&]() {
			SpawnEffect->Deactivate();
			Destroy();
		});

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DestroyDelayTime, false);
	}
}

