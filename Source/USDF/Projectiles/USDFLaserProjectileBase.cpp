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

	RootComponent = Mesh;
	BaseEffect->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AUSDFLaserProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	BaseEffect->Activate();
	BaseEffect->SetAutoDestroy(true);
}

void AUSDFLaserProjectileBase::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called every frame
void AUSDFLaserProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

