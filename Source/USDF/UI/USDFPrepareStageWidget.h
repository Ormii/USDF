// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFPrepareStageWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFPrepareStageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Meta = (DisplayName = "OnActivatePrepareStageCpp"))
	void OnActivatePrepareStage();
};
