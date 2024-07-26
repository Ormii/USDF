// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageDotDamageZone.h"
#include "Components/HeterogeneousVolumeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/USDFCollision.h"
#include "Interface/USDFDamageableInterface.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/USDFDamageableInterface.h"

AUSDFDarkMageDotDamageZone::AUSDFDarkMageDotDamageZone()
{
	//CDO
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	HeterogeneousVolume = CreateDefaultSubobject<UHeterogeneousVolumeComponent>(TEXT("HeterogenousVolume"));
	
	HeterogeneousVolume->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatRef(TEXT("/Game/ReferenceAsset/WeirdFlameVDB/Materials/MI_WeirdFlame_01.MI_WeirdFlame_01"));
	if (MatRef.Object)
	{
		HeterogeneousVolume->SetMaterial(0,MatRef.Object);
	}

	TagID = 0;
	ActivateBeginTime = 0;
	CurrentTime = 0;
}

void AUSDFDarkMageDotDamageZone::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentTime = 0;
	bActivate = false;

	Mesh->OnComponentHit.AddDynamic(this, &AUSDFDarkMageDotDamageZone::OnComponentHitFunc);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AUSDFDarkMageDotDamageZone::OnComponentOverlapBeginFunc);
}

void AUSDFDarkMageDotDamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActivate)
	{
		CurrentTime += DeltaTime;

		if (CurrentTime >= DotInterval)
		{
			CurrentTime -= DotInterval;
			
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams Params(SCENE_QUERY_STAT(DotDamageOverlap), false, this);

			bool bIsOverlapped = GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(),
				FQuat::Identity, CCHANNEL_USDF_NONPLAYERACTION, FCollisionShape::MakeSphere(DotRange), Params);

			if (bIsOverlapped)
			{
				for (int32 i = 0; i < OverlapResults.Num(); ++i)
				{
					IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(OverlapResults[i].GetActor());
					if (Damageable == nullptr)
						continue;

					FDamageInfo DamageInfo{};
					DamageInfo.DamageAmount = DotDamage;
					DamageInfo.DamageCauser = this;
					DamageInfo.DamageType = EDamageType::DotDamage;

					Damageable->TakeDamage(DamageInfo);
				}
			}

#if ENABLE_DRAW_DEBUG
			//DrawDebugSphere(GetWorld(), GetActorLocation(), DotRange, 20.0f, FColor::Green, false, 0.1f);
#endif
		}
	}
}

void AUSDFDarkMageDotDamageZone::BeginActivate()
{
	bActivate = true;
}

void AUSDFDarkMageDotDamageZone::OnComponentHitFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileMovement->Deactivate();
	SphereCollision->SetGenerateOverlapEvents(false);
	HitCharacters.Empty();
	BeginActivate();

	OnHitEventAccur.ExecuteIfBound();
}

void AUSDFDarkMageDotDamageZone::OnComponentOverlapBeginFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitCharacters.Find(OtherActor) != nullptr)
		return;

	IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(OtherActor);
	if (Damageable)
	{
		FDamageInfo DamageInfo{};
		DamageInfo.DamageAmount = CollisionDamage;
		DamageInfo.DamageCauser = this;
		DamageInfo.DamageType = EDamageType::HitDefault;
	
		Damageable->TakeDamage(DamageInfo);

		HitCharacters.Add(OtherActor);
	}
}
	