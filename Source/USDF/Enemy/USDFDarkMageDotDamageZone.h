// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/USDFDotDamageZone.h"
#include "USDFDarkMageDotDamageZone.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageDotDamageZone : public AUSDFDotDamageZone
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageDotDamageZone();

protected:
	UPROPERTY(VisibleAnywhere, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHeterogeneousVolumeComponent > HeterogeneousVolume;
};
