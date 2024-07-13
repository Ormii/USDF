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
	CurrentStageNumber = 0;
	MaxStageNumber = 5;
}

void AUSDFGameStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameStageChangeManager.Add(EGameStageState::Prepare, FOnGameStageChange::CreateUObject(this, &AUSDFGameStage::PrepareGameStage));
	GameStageChangeManager.Add(EGameStageState::Battle, FOnGameStageChange::CreateUObject(this, &AUSDFGameStage::BattleGameStage));
	GameStageChangeManager.Add(EGameStageState::End, FOnGameStageChange::CreateUObject(this, &AUSDFGameStage::EndGameStage));
}

// Called when the game starts or when spawned
void AUSDFGameStage::BeginPlay()
{
	Super::BeginPlay();
	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnGoalCompleteDelegate().AddUObject(this, &AUSDFGameStage::CompleteGoal);
		GameModeInterface->SetGameStage(this);
	}
}

// Called every frame
void AUSDFGameStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentGameStage == EGameStageState::Battle)
	{
		IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
		if (GameModeInterface && GameModeInterface->IsCompleteGoal())
		{
			SetGameStageState(EGameStageState::End);
		}
	}
}

void AUSDFGameStage::SetGameStageState(EGameStageState NewGameStageState)
{
	CurrentGameStage = NewGameStageState;
	GameStageChangeManager[CurrentGameStage].OnGameStageChange.ExecuteIfBound();
}

void AUSDFGameStage::PrepareGameStage()
{
	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	
	if (GameModeInterface == nullptr)
		return;

	CurrentStageNumber++;

	TMap<UClass*, int> Goals;

	for (int32 i = 0; i < AISpawners.Num(); ++i)
	{
		AISpawners[i]->PrepareSpawn(CurrentStageNumber);
		AISpawners[i]->SetGoal(Goals);
	}

	GameModeInterface->SetGameModeGoal(FGameModeGoalParam(Goals));
}

void AUSDFGameStage::BattleGameStage()
{
	for (int32 i = 0; i < AISpawners.Num(); ++i)
	{
		AISpawners[i]->BeginSpawn();
	}
}

void AUSDFGameStage::EndGameStage()
{
	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface == nullptr)
		return;

	GameModeInterface->EndGameStage();
}

void AUSDFGameStage::CompleteGoal()
{
	SetGameStageState(EGameStageState::End);
}

