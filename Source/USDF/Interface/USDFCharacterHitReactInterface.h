// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USDFCharacterHitReactInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSDFCharacterHitReactInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class USDF_API IUSDFCharacterHitReactInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HitReact(const FHitResult& HitResult, const float DamageAmount, const AActor* HitCauser) = 0;
	virtual bool GetHitReactState() = 0;
};
