// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFEndStageWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFEndStageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Meta = (DisplayName = "OnActivateEndStageCpp"))
	void OnActivateEndStage();
};
