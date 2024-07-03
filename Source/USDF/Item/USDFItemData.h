// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USDFItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
};

/**
 * 
 */
UCLASS()
class USDF_API UUSDFItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		FString NameAndTag = FString::Printf(TEXT("%s:%s"), TEXT("USDFItemData"), *GetName());
		return FPrimaryAssetId(NameAndTag);
	}

protected:
	UPROPERTY(EditAnywhere, Category = Type)
	EItemType ItemType;
};
