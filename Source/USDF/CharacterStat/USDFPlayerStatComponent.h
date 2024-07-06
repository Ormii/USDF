// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USDFCharacterStatComponent.h"
#include "GameData/USDFPlayerStat.h"
#include "USDFPlayerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USDF_API UUSDFPlayerStatComponent : public UUSDFCharacterStatComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSDFPlayerStatComponent();


	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUSDFPlayerStat PlayerStat;

public:
	void InitPlayerStat(const FUSDFPlayerStat& NewPlayerStat);

	FORCEINLINE const FUSDFPlayerStat& GetPlayerStat() const { return PlayerStat; }
};
