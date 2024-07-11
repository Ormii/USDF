// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USDFItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AUSDFItemBase::AUSDFItemBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MainEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainEffect"));
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));

	RootComponent = Mesh;
	MainEffect->SetupAttachment(RootComponent);
	TrailEffect->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
}
