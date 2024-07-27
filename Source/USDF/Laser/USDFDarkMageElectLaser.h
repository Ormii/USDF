// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Laser/USDFEnemyLaser.h"
#include "USDFDarkMageElectLaser.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageElectLaser : public AUSDFEnemyLaser
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageElectLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	FTimerHandle TimerHandle;

protected:
	float Distance;

	bool bRotate;
	float AcculmulateAngle;
};
