// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/USDFEnemyLaserProjectile.h"
#include "USDFDarkMageUpLaserProjectile.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageUpLaserProjectile : public AUSDFEnemyLaserProjectile
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageUpLaserProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void BoxCollisionActivate();
	void BoxCollisionDeActivate();
};
