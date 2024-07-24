// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/USDFGameStage.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"
#include "AI/USDFAISpawner.h"

// Sets default values
AUSDFGameStage::AUSDFGameStage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUSDFGameStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AUSDFGameStage::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUSDFGameStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
