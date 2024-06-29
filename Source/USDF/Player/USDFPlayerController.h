// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "USDFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUSDFPlayerController();

protected:
	virtual void BeginPlay() override;
};
