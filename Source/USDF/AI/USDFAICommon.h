#pragma once

#include "CoreMinimal.h"
#include "USDFAICommon.generated.h"

#define BBKEY_TARGET TEXT("Target")
#define BBKEY_HOMEPOS TEXT("HomePos")
#define BBKEY_PATROLPOS TEXT("NextPatrolPos")
#define BBKEY_HITREACTSTATE TEXT("HitReactState")
#define BBKEY_AISTATE TEXT("State")
#define BBKEY_POINT_OF_INTREST TEXT("PointOfInterest")

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
};