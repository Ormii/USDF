#pragma once

#include "CoreMinimal.h"
#include "Damage/USDFDamageCommon.h"
#include "USDFAICommon.generated.h"

#define BBKEY_AISTATE TEXT("State")
#define BBKEY_POINT_OF_INTREST TEXT("PointOfInterest")
#define BBKEY_ATTACK_TARGET TEXT("AttackTarget")
#define BBKEY_EQS_TARGET_RADIUS TEXT("EQSTargetRadius")
#define BBKEY_EQS_DEFEND_RADIUS TEXT("EQSDefendRadius")
#define BBKEY_GAME_STAGE_PHASE TEXT("GameStagePhase")

struct FAISensedParam;

DECLARE_DELEGATE_OneParam(FOnChangeAIState, FAISensedParam /*Target*/)

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EAISense : uint8
{
	None UMETA(DisplayName = "None"),
	Sight UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Damage UMETA(DisplayName = "Damage"),
};

USTRUCT()
struct FAISensedParam
{
	GENERATED_BODY()

	TObjectPtr<AActor> Actor;
	FVector Location;
	FDamageInfo DamageInfo;

	bool bUseLastAttackTarget;
};