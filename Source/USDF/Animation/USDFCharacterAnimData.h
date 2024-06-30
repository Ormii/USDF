// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USDFCharacterAnimData.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFCharacterAnimData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UUSDFCharacterAnimData();

	UPROPERTY(EditAnywhere, Category = Threshould)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, Category = Threshould)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, Category = Threshould)
	float WalkingThreshould;

	UPROPERTY(EditAnywhere, Category = Threshould)
	float RunningThreshould;
};
