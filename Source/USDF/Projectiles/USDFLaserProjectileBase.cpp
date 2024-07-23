// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFLaserProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Interface/USDFDamageableInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUSDFLaserProjectileBase::AUSDFLaserProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseEffect"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	RootComponent = Mesh;
	BaseEffect->SetupAttachment(RootComponent);
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUSDFLaserProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AUSDFLaserProjectileBase::OnOverlapBeginFunc);
	BaseEffect->Activate();
	BaseEffect->SetAutoDestroy(true);
}

// Called every frame
void AUSDFLaserProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if ENABLE_DRAW_DEBUG
	if(BoxCollision->GetVisibleFlag())
		DrawDebugBox(GetWorld(), BoxCollision->GetComponentLocation(), BoxCollision->GetScaledBoxExtent(), FColor::Green, false, 0.1f);
#endif
}



void AUSDFLaserProjectileBase::OnOverlapBeginFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitCharacters.Find(OtherActor) != nullptr)
		return;

	IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(OtherActor);
	if (Damageable == nullptr)
		return;

	FDamageInfo DamageInfo{};
	DamageInfo.DamageAmount = AttackDamage;
	DamageInfo.DamageCauser = GetOwner();
	DamageInfo.DamageType = EDamageType::HitDefault;

	Damageable->TakeDamage(DamageInfo);

	HitCharacters.Add(OtherActor);

	UE_LOG(LogTemp, Display, TEXT("Hit Character!!!!!!"));
}

