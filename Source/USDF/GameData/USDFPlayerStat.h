#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "USDFPlayerStat.generated.h"

USTRUCT(BlueprintType)
struct FUSDFPlayerStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FUSDFPlayerStat() :MaxHp(0.0f), DefaultAttack(0.0f) {}
	FUSDFPlayerStat(float InMaxHp, float InDefaultAttack) : MaxHp(InMaxHp), DefaultAttack(InDefaultAttack){}

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float DefaultAttack;

	UPROPERTY()
	float MaxWalkSpeed;

	UPROPERTY()
	float MaxRunSpeed;

};