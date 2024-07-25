#pragma once

#include "CoreMinimal.h"
#include "USDFDamageCommon.generated.h"

UENUM()
enum class EDamageType : uint8
{
	HitDefault,
	HitKnockback,
};

USTRUCT()
struct FDamageInfo
{
	GENERATED_BODY()

public:
	FDamageInfo() {}

	float DamageAmount;
	EDamageType DamageType;

	UPROPERTY()
	TObjectPtr<AActor> DamageCauser;
};