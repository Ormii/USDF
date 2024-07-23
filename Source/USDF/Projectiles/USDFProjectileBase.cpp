// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Interface/USDFDamageableInterface.h"

// Sets default values
AUSDFProjectileBase::AUSDFProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	BaseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseEffect"));

	RootComponent = Mesh;
	BaseEffect->SetupAttachment(RootComponent);
}

void AUSDFProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AUSDFProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &AUSDFProjectileBase::OnComponentHitFunc);
	BaseEffect->Activate();
}

// Called every frame
void AUSDFProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUSDFProjectileBase::OnComponentHitFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(OtherActor);
	if (Damageable)
	{
		FDamageInfo DamageInfo = {};
		DamageInfo.DamageAmount = AttackDamage;
		DamageInfo.DamageType = EDamageType::HitDefault;
		DamageInfo.DamageCauser = GetOwner();

		Damageable->TakeDamage(DamageInfo);
	}

	FVector HitEftSpawnLocation = Hit.Location;
	FRotator HitEftSpawnRotation = GetActorRotation();

	if (HitEffect != nullptr)
	{
		UNiagaraComponent* pNiagaraCompo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, HitEftSpawnLocation, HitEftSpawnRotation);
		if (pNiagaraCompo != nullptr)
		{
			pNiagaraCompo->Activate();
		}
	}

	BaseEffect->Deactivate();
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle TimerHandle{};
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&]() {Destroy(); });
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerCallback, 4.0f, false);
}