#pragma once
#include "CoreMinimal.h"
#include "USDFLocomotionState.generated.h"

UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Run  UMETA(DisplayName = "Run"),
};