// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/USDFGameSingleton.h"
#include "USDFNormalMonsterStat.h"
#include "USDFBossMonsterStat.h"
#include "USDFPlayerStat.h"

DEFINE_LOG_CATEGORY(USDFGameSingleton);

UUSDFGameSingleton::UUSDFGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> NormalMonsterStatDataTableRef(TEXT("/Game/GameData/DT_USDF_NormalMonsterStat.DT_USDF_NormalMonsterStat"));
	if (NormalMonsterStatDataTableRef.Object)
	{
		const UDataTable* DataTable = NormalMonsterStatDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		FString ContextString;
		for (int i = 0; i < DataTable->GetRowMap().Num(); ++i)
		{
			FUSDFNormalMonsterStat* NormalMonsterStat = DataTable->FindRow<FUSDFNormalMonsterStat>(DataTable->GetRowNames()[i], ContextString);
			NormalMonsterStatTable.Add(DataTable->GetRowNames()[i], *NormalMonsterStat);
		}
	}

	
	static ConstructorHelpers::FObjectFinder<UDataTable> BossMonsterStatDataTableRef(TEXT("/Game/GameData/DT_USDF_BossMonsterStat.DT_USDF_BossMonsterStat"));
	if (NormalMonsterStatDataTableRef.Object)
	{
		const UDataTable* DataTable = BossMonsterStatDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		FString ContextString;
		for (int i = 0; i < DataTable->GetRowMap().Num(); ++i)
		{
			FUSDFBossMonsterStat* BossMonsterStat = DataTable->FindRow<FUSDFBossMonsterStat>(DataTable->GetRowNames()[i], ContextString);
			BossMonsterStatTable.Add(DataTable->GetRowNames()[i], *BossMonsterStat);
		}
	}
	

	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerStatDataTableRef(TEXT("/Game/GameData/USDFPlayerStatTable.USDFPlayerStatTable"));
	if(PlayerStatDataTableRef.Object)
	{
		const UDataTable* DataTable = PlayerStatDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		FString ContextString;
		for (int i = 0; i < DataTable->GetRowMap().Num(); ++i)
		{
			FUSDFPlayerStat* PlayerStat = DataTable->FindRow<FUSDFPlayerStat>(DataTable->GetRowNames()[i], ContextString);
			PlayerStatTable.Add(DataTable->GetRowNames()[i], *PlayerStat);
		}
	}


}

const UUSDFGameSingleton& UUSDFGameSingleton::Get()
{
	static UUSDFGameSingleton* Instance = CastChecked<UUSDFGameSingleton>(GEngine->GameSingleton);
	if (Instance)
	{
		return *Instance;
	}

	UE_LOG(USDFGameSingleton, Error, TEXT("USDGameSingleton Failed"));
	return *NewObject<UUSDFGameSingleton>();
}
