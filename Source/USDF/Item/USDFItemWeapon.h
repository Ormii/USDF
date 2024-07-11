// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/USDFItemBase.h"
#include "USDFItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFItemWeapon : public AUSDFItemBase
{
	GENERATED_BODY()

public:
	AUSDFItemWeapon();

	//Weapon Data Section
protected:
	TObjectPtr<class UUSDFWeaponItemData> WeaponItemData;

public:
	FORCEINLINE class UUSDFWeaponItemData* GetWeaponItemData() const { return WeaponItemData; }
};
