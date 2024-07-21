// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/USDFCharacterStatComponent.h"
#include "GameData/USDFBossMonsterStat.h"
#include "USDFBossMonsterStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFBossMonsterStatComponent : public UUSDFCharacterStatComponent
{
	GENERATED_BODY()

public:
	UUSDFBossMonsterStatComponent();

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUSDFBossMonsterStat BossMonsterStat;

public:
	void InitBossMonsterStat(const FUSDFBossMonsterStat& NewBossMonsterStat);

	FORCEINLINE const FUSDFBossMonsterStat& GetBossMonsterStat() const { return BossMonsterStat; }
};