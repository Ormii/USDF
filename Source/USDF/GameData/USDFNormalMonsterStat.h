#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "USDFNormalMonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FUSDFNormalMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FUSDFNormalMonsterStat() :MaxHp(0.0f), DefaultAttack(0.0f), StrongAttack(0.0f), WalkSpeed(0.0f), RunSpeed(0.0f), PatrolRadius(0.0f), DetectRadius(0.0f), AttackRange(0.0f), TurnRateSpeed(0.0f){}

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float DefaultAttack;

	UPROPERTY()
	float StrongAttack;

	UPROPERTY()
	float WalkSpeed;

	UPROPERTY()
	float RunSpeed;

	UPROPERTY()
	float PatrolRadius;

	UPROPERTY()
	float DetectRadius;

	UPROPERTY()
	float AttackRange;

	UPROPERTY()
	float EQSTargetRange;

	UPROPERTY()
	float EQSDefendRange;

	UPROPERTY()
	float TurnRateSpeed;
};