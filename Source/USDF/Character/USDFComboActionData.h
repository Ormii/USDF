// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USDFComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UUSDFComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	int8	MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float	FrameRate;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
