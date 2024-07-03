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

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSoftObjectPtr<class UStaticMesh> Mesh;
};
