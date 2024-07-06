// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "USDFNormalMonsterStat.h"
#include "USDFPlayerStat.h"
#include "USDFGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(USDFGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class USDF_API UUSDFGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UUSDFGameSingleton();

	const static UUSDFGameSingleton& Get();


public:
	FORCEINLINE const FUSDFNormalMonsterStat GetNormalMonsterStat(FName KeyName)
	{
		if (NormalMonsterStatTable.Find(KeyName) != nullptr)
		{
			return NormalMonsterStatTable[KeyName];
		}

		UE_LOG(LogTemp, Display, TEXT("Not Found NormalMonster Stat : %s"), *KeyName.ToString());
			
		return FUSDFNormalMonsterStat();
	}

	FORCEINLINE const FUSDFPlayerStat GetPlayerStat(FName KeyName)
	{
		if (PlayerStatTable.Find(KeyName) != nullptr)
			return PlayerStatTable[KeyName];


		UE_LOG(LogTemp, Display, TEXT("Not Found Player Stat : %s"), *KeyName.ToString());


		return FUSDFPlayerStat();
	}

private:
	TMap<FName, FUSDFNormalMonsterStat> NormalMonsterStatTable;
	TMap<FName, FUSDFPlayerStat> PlayerStatTable;
};
