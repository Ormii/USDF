// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFPatrolRoute.h"
#include "Components/SplineComponent.h"

// Sets default values
AUSDFPatrolRoute::AUSDFPatrolRoute()
{
	PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
	PatrolIndex = 0;
	Direction = 1;
}

void AUSDFPatrolRoute::IncrementPatrolRoute()
{
	PatrolIndex += Direction;
	if (PatrolIndex == PatrolRoute->GetNumberOfSplinePoints() - 1)
		Direction = -1;
	else if (PatrolIndex == 0)
		Direction = 1;
}

FVector AUSDFPatrolRoute::GetSplinePointAsWorldPosition()
{
	return PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}
