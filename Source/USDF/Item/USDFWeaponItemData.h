// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/USDFItemData.h"
#include "USDFWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFWeaponItemData : public UUSDFItemData
{
	GENERATED_BODY()
	
public:
	UUSDFWeaponItemData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		FString NameAndTag = FString::Printf(TEXT("%s:%s"), TEXT("USDFItemData"), *GetName());
		return FPrimaryAssetId(NameAndTag);
	}


protected:
	UPROPERTY(EditAnywhere, Category = WeaponEffect)
	TObjectPtr<class UParticleSystem> SwordEffect;

	UPROPERTY(EditAnywhere, Category = WeaponEffect)
	TObjectPtr<class UParticleSystem> TrailEffect;

	UPROPERTY(EditAnywhere, Category = WeaponEffect)
	FName TrailStart;

	UPROPERTY(EditAnywhere, Category = WeaponEffect)
	FName TrailEnd;

public:
	FORCEINLINE class UParticleSystem* GetSwordEffect() { return SwordEffect; }
	FORCEINLINE class UParticleSystem* GetTrailEffect() { return TrailEffect; }

	FORCEINLINE FName GetTrailStart() { return TrailStart; }
	FORCEINLINE FName GetTrailEnd() { return TrailEnd; }
};
