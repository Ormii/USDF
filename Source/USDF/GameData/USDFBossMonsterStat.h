#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "USDFBossMonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FUSDFBossMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FUSDFBossMonsterStat() :MaxHp(0.0f), DefaultAttack(0.0f), Skill1(0.0f), Skill2(0.0f), Skill3(0.0f), WalkSpeed(0.0f), RunSpeed(0.0f), AttackRange(0.0f), EQSDefendRange(0.0f), TurnRateSpeed(0.0f){}

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float DefaultAttack;

	UPROPERTY()
	float Skill1;

	UPROPERTY()
	float Skill2;

	UPROPERTY()
	float Skill3;

	UPROPERTY()
	float WalkSpeed;

	UPROPERTY()
	float RunSpeed;

	UPROPERTY()
	float AttackRange;

	UPROPERTY()
	float EQSDefendRange;

	UPROPERTY()
	float TurnRateSpeed;
};