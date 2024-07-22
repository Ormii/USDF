// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFDarkMageDefaultProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AUSDFDarkMageDefaultProjectile::AUSDFDarkMageDefaultProjectile()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BaseEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Orb/NS_DarkMagic_Orb.NS_DarkMagic_Orb"));
	if (BaseEffectRef.Object)
	{
		BaseEffect->SetAsset(BaseEffectRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Projectile/NS_Explosion_Grenade_DarkMagic.NS_Explosion_Grenade_DarkMagic"));
	if (HitEffectRef.Object)
	{
		HitEffect = HitEffectRef.Object;
	}
}
