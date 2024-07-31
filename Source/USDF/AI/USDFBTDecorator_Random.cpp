// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBTDecorator_Random.h"

UUSDFBTDecorator_Random::UUSDFBTDecorator_Random()
{
    NodeName = "Random Decorator";
    Probability = 0.5f;
}

bool UUSDFBTDecorator_Random::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return FMath::FRand() < Probability;
}
