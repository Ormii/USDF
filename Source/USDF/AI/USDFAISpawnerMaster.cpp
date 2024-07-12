// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFAISpawnerMaster.h"
#include "AI/USDFAISpawner.h"

// Sets default values
AUSDFAISpawnerMaster::AUSDFAISpawnerMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUSDFAISpawnerMaster::BeginPlay()
{
	Super::BeginPlay();
	PrepareSpawnOrder(1);
	BeginSpawnOrder();
}

// Called every frame
void AUSDFAISpawnerMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUSDFAISpawnerMaster::PrepareSpawnOrder(int32 StageNumber)
{
	for (int32 i = 0; i < AISpawners.Num(); ++i)
	{
		AISpawners[i]->PrepareSpawn(StageNumber);
	}
}

void AUSDFAISpawnerMaster::BeginSpawnOrder()
{
	for (int32 i = 0; i < AISpawners.Num(); ++i)
	{
		AISpawners[i]->BeginSpawn();
	}
}

