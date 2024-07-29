// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFGameStageIntroWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFGameStageIntroWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnIntroStartCpp"))
	void K2_OnIntroStart();
};
