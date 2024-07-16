// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFPatrolRoute.generated.h"

UCLASS()
class USDF_API AUSDFPatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	AUSDFPatrolRoute();

public:
	UFUNCTION(BlueprintCallable)
	void IncrementPatrolRoute();

	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointAsWorldPosition();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Route, Meta = (AllowPrivateAccess="true"))
	TObjectPtr<class USplineComponent> PatrolRoute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Route, Meta = (AllowPrivateAccess= "true"))
	int32 PatrolIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Route, Meta = (AllowPrivateAccess = "true"))
	int32 Direction;
};
