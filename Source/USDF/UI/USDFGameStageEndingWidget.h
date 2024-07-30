// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFGameStageEndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFGameStageEndingWidget : public UUserWidget
{
	GENERATED_BODY()

	//Retry Section
protected:
	UFUNCTION(BlueprintCallable, Category = Retry, Meta = (DisplayName = "OnStageEndingRetryButtonEventCpp"))
	void K2_OnStageEndingRetryButtonEvent();
};
