// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFDarkMageMeteoProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AUSDFDarkMageMeteoProjectile::AUSDFDarkMageMeteoProjectile()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BaseEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Meteor/NS_Meteor_DarkMagic.NS_Meteor_DarkMagic"));
	if (BaseEffectRef.Object)
	{
		BaseEffect->SetAsset(BaseEffectRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Meteor/NS_Explosion_DarkMagic.NS_Explosion_DarkMagic"));
	if (HitEffectRef.Object)
	{
		HitEffect = HitEffectRef.Object;
	}
}
