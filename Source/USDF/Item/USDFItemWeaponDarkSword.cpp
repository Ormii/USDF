// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USDFItemWeaponDarkSword.h"
#include "Item/USDFWeaponItemData.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

AUSDFItemWeaponDarkSword::AUSDFItemWeaponDarkSword()
{
	static ConstructorHelpers::FObjectFinder<UUSDFWeaponItemData> WeaponItemDataRef(TEXT("/Game/Item/DA_USDF_DarkSword_ItemData.DA_USDF_DarkSword_ItemData"));
	if (WeaponItemDataRef.Object)
	{
		WeaponItemData = WeaponItemDataRef.Object;

		Mesh->SetStaticMesh(WeaponItemData->GetMesh());
		MainEffect->SetTemplate(WeaponItemData->GetSwordEffect());
		TrailEffect->SetTemplate(WeaponItemData->GetTrailEffect());

		MainEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}
}