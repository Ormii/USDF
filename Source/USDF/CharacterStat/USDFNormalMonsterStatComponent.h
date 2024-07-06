// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USDFCharacterStatComponent.h"
#include "GameData/USDFNormalMonsterStat.h"
#include "USDFNormalMonsterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USDF_API UUSDFNormalMonsterStatComponent :  public UUSDFCharacterStatComponent

{
	GENERATED_BODY()

public:	
	UUSDFNormalMonsterStatComponent();

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUSDFNormalMonsterStat NormalMonsterStat;

public:
	void InitNormalMonsterStat(const FUSDFNormalMonsterStat& NewNormalMonsterStat);

	FORCEINLINE const FUSDFNormalMonsterStat& GetNormalMonsterStat() const { return NormalMonsterStat; }
};
