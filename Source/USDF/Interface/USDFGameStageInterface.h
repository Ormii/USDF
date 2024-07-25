// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USDFGameStageInterface.generated.h"

UENUM(BlueprintType)
enum class EGameStagePhase : uint8
{
	EGameStagePhase_Intro,
	EGameStagePhase_Phase1,
	EGameStagePhase_Phase2,
	EGameStagePhase_Phase3,
	EGameStagePhase_Ending
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUSDFGameStageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class USDF_API IUSDFGameStageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetGameStage(EGameStagePhase NewGameStagePhase) = 0;
};
