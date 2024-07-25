// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageDotDamageZone.h"
#include "Components/HeterogeneousVolumeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/USDFCollision.h"

AUSDFDarkMageDotDamageZone::AUSDFDarkMageDotDamageZone()
{
	Mesh->SetCollisionProfileName(CPROFILE_USDF_TRIGGER);

	//CDO
	HeterogeneousVolume = CreateDefaultSubobject<UHeterogeneousVolumeComponent>(TEXT("HeterogenousVolume"));
	HeterogeneousVolume->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatRef(TEXT("/Game/ReferenceAsset/WeirdFlameVDB/Materials/MI_WeirdFlame_01.MI_WeirdFlame_01"));
	if (MatRef.Object)
	{
		HeterogeneousVolume->SetMaterial(0,MatRef.Object);
	}
}
	