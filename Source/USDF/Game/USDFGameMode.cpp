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

void AUSDFGameMode::OnGameStageIntro()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->K2_OnGameStageIntro();
	}
}

void AUSDFGameMode::OnGameStageEnding()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->K2_OnGameStageEnding();
	}
}


void AUSDFGameMode::OnGameRetry()
{
	UWorld* World = GetWorld();
	FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(GetWorld());

	UGameplayStatics::OpenLevel(World, CurrentLevelName);
}
