// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USDFCharacterPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSDFCharacterPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class USDF_API IUSDFCharacterPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual APlayerController* GetPlayerController() = 0;
};
