// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USDFAIOrderSetData.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFAIOrderSetData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = AIOrderSet, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFAIController> AIControllerClass;
};
