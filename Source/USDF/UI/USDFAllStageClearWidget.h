// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFAllStageClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFAllStageClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnActivateAllStageClearCpp"))
	void OnActivateAllStageClear();

	//Retry Section
protected:
	UFUNCTION(BlueprintCallable, Category = Retry, Meta = (DisplayName = "OnAllStageClearRetryButtonEventCpp"))
	void K2_OnAllStageClearRetryButtonEvent();
};
