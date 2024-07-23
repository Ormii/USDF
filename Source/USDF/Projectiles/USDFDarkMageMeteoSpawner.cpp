// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFDarkMageMeteoSpawner.h"
#include "Projectiles/USDFDarkMageMeteoProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
AUSDFDarkMageMeteoSpawner::AUSDFDarkMageMeteoSpawner()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SpawnEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Meteor/NS_Portal_Meteor_DarkMagic.NS_Portal_Meteor_DarkMagic"));
	if (SpawnEffectRef.Object)
	{
		SpawnEffect->SetAsset(SpawnEffectRef.Object);
	}

	static ConstructorHelpers::FClassFinder<AUSDFDarkMageMeteoProjectile> ProjectileClassRef(TEXT("/Game/Blueprint/Projectiles/BP_USDFDarkMageMeteoProjectile.BP_USDFDarkMageMeteoProjectile_C"));
	if (ProjectileClassRef.Class)
	{
		ProjectileClass = ProjectileClassRef.Class;
	}

	SpawnDelayTime = 1.0f;
	DestroyDelayTime = 3.0f;

	ProjectileInitSpeed = 2500.0f;
}