// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDotDamageZone.h"
#include "Components/SphereComponent.h"

// Sets default values
AUSDFDotDamageZone::AUSDFDotDamageZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	RootComponent = Mesh;
	SphereCollision->SetupAttachment(RootComponent);

	DotInterval = 1.0f;
	DotDamage = 0.0f;
}

// Called when the game starts or when spawned
void AUSDFDotDamageZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUSDFDotDamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

