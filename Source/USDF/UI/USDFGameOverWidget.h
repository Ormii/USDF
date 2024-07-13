// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//Retry Section
protected:
	UFUNCTION(BlueprintCallable, Category = Retry, Meta = (DisplayName = "OnGameOverRetryButtonEventCpp"))
	void K2_OnGameOverRetryButtonEvent();
};
