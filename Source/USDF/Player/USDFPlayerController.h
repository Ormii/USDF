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

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD, Meta = (AllowPrivateAccess= "true"))
	TSubclassOf<class UUSDFPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD, Meta= (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFPlayerHUDWidget> HUDWidget;
};

