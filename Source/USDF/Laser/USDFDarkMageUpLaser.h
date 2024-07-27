// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Laser/USDFEnemyLaser.h"
#include "USDFDarkMageUpLaser.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageUpLaser : public AUSDFEnemyLaser
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageUpLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	void CollisionActivate();

protected:
	UPROPERTY()
	FTimerHandle TimerHandle;

	bool bCollisionActivate;
	float ActivateTime;

	float LaserLength;
};
