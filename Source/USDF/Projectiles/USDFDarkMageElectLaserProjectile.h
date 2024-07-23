// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/USDFEnemyLaserProjectile.h"
#include "USDFDarkMageElectLaserProjectile.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageElectLaserProjectile : public AUSDFEnemyLaserProjectile
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageElectLaserProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	float Distance;

	bool bRotate;
	float AcculmulateAngle;
};
