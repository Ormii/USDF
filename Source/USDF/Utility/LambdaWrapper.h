// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "LambdaWrapper.generated.h"


/**
 * 
 */
UCLASS()
class USDF_API ULambdaWrapper : public UObject
{
	GENERATED_BODY()
public:
	TFunction<void(FAIRequestID, EPathFollowingResult::Type)> OnMoveCompleteCallFn;

	UFUNCTION()
	void OnMoveCompleteDispatch(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
