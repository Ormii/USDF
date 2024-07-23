// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/USDFGameMode.h"
#include "Player/USDFPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/USDFGameStage.h"
#include "Character/USDFCharacterBossMonster.h"

AUSDFGameMode::AUSDFGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> CharacterPlayerRef(TEXT("/Game/Blueprint/BP_USDFCharacterPlayerWarrior.BP_USDFCharacterPlayerWarrior_C"));
	if (CharacterPlayerRef.Class)
	{
		DefaultPawnClass = CharacterPlayerRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Game/Blueprint/BP_USDFPlayerController.BP_USDFPlayerController_C"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
}

void AUSDFGameMode::OnPlayerDead()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->K2_OnGameOver();
	}
}

void AUSDFGameMode::OnBossEntry(AUSDFCharacterBossMonster* BossMonster)
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->SetTargetBoss(BossMonster);
	}
}

void AUSDFGameMode::OnBossDead(AUSDFCharacterBossMonster* BossMonster)
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->OnBossDead(BossMonster);
	}
}

void AUSDFGameMode::SetGameStage(AUSDFGameStage* NewGameStage)
{
	GameStage = NewGameStage;
}

bool AUSDFGameMode::IsCompleteGoal()
{
	if (Target.Goals.Num() != Current.Goals.Num())
		return false;

	for (auto& Goal : Target.Goals)
	{
		if (Current.Goals.Find(Goal.Key) == nullptr)
			return false;
		if (Goal.Value != Current.Goals[Goal.Key])
			return false;
	}

	return true;
}

void AUSDFGameMode::UpdateCurrent(UClass* Class)
{
	IncreaseCurrentGoalValue(Class);
}

void AUSDFGameMode::PrepareGameStage()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		if (GameStage)
		{
			int CurrentStageNumber = GameStage->GetCurrentStageNumber();
			int MaxStageNumber = GameStage->GetMaxStageNumber();
			if (CurrentStageNumber + 1 > MaxStageNumber)
			{
				PlayerController->K2_OnAllGameStageClear();
			}
			else
			{
				GameStage->SetGameStageState(EGameStageState::Prepare);
				PlayerController->K2_OnPrepareGameStage();
			}
		}
	}
}

void AUSDFGameMode::EndGameStage()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->K2_OnEndGameStage();
	}
}

void AUSDFGameMode::BattleGameStage()
{
	if (GameStage)
	{
		GameStage->SetGameStageState(EGameStageState::Battle);
	}
}

void AUSDFGameMode::OnGameRetry()
{
	UWorld* World = GetWorld();
	FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(GetWorld());

	UGameplayStatics::OpenLevel(World, CurrentLevelName);
}

void AUSDFGameMode::IncreaseCurrentGoalValue(UClass* Class)
{
	if (Current.Goals.Find(Class) == nullptr)
		Current.Goals.Add(Class, 0);

	Current.Goals[Class]++;
}

void AUSDFGameMode::SetGameModeGoal(FGameModeGoalParam&& NewGoal)
{
	Target = NewGoal;
	Current.Goals.Empty();
	for (auto Goal : Target.Goals)
	{
		Current.Goals.Add(Goal.Key, 0);
	}
}
