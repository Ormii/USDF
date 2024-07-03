// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USDFUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUSDFUserWidget(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE void SetOwner(AActor* NewOwner) { Owner = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere, Category = Actor, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<AActor> Owner;
};
