// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/USDFBossDarkMageGameStage.h"
#include "LevelSequencePlayer.h"
#include "Interface/USDFGameModeInterface.h"
#include "Interface/USDFDamageableInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Game/USDFGameMode.h"
#include "Player/USDFPlayerController.h"
#include "AI/USDFAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/USDFCharacterPlayer.h"
#include "Character/USDFCharacterBossDarkMage.h"

AUSDFBossDarkMageGameStage::AUSDFBossDarkMageGameStage()
{
	// CDO

	static ConstructorHelpers::FObjectFinder<ULevelSequence> Phase1SeqAssetRef(TEXT("/Game/Cinematics/BossIntro/BossIntro.BossIntro"));
	if (Phase1SeqAssetRef.Object)
	{
		Phase1SeqAsset = Phase1SeqAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<ULevelSequence> Phase2SeqAssetRef(TEXT("/Game/Cinematics/BossMiddlePhase/BossMiddlePhase.BossMiddlePhase"));
	if (Phase2SeqAssetRef.Object)
	{
		Phase2SeqAsset = Phase2SeqAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<ULevelSequence> Phase3SeqAssetRef(TEXT(""));
	if (Phase3SeqAssetRef.Object)
	{
		Phase3SeqAsset = Phase3SeqAssetRef.Object;
	}

	CurrentDarkMageStagePhase = EDarkMageStagePhase::EDarkMageStagePhase_Intro;

}

void AUSDFBossDarkMageGameStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AUSDFBossDarkMageGameStage::BeginPlay()
{
	Super::BeginPlay();

	AActor* Player = UGameplayStatics::GetActorOfClass(this, AUSDFCharacterPlayer::StaticClass());
	PlayerCharacter = Cast<AUSDFCharacterPlayer>(Player);
	check(PlayerCharacter);

	AActor* Boss = UGameplayStatics::GetActorOfClass(this, AUSDFCharacterBossDarkMage::StaticClass());
	BossDarkMage = Cast<AUSDFCharacterBossDarkMage>(Boss);
	check(BossDarkMage);


	DarkMageStagePhaseManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Intro,
		FDarkMageStagePhaseWrapper(FOnDarkMageStagePhaseChange::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangeIntro),
			FOnDarkMageStagePhaseUpdate::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdateIntro)) });

	DarkMageStagePhaseManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Phase1,
		FDarkMageStagePhaseWrapper(FOnDarkMageStagePhaseChange::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase1),
			FOnDarkMageStagePhaseUpdate::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase1)) });

	DarkMageStagePhaseManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Phase2,
		FDarkMageStagePhaseWrapper(FOnDarkMageStagePhaseChange::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase2),
			FOnDarkMageStagePhaseUpdate::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase2)) });

	DarkMageStagePhaseManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Phase3,
		FDarkMageStagePhaseWrapper(FOnDarkMageStagePhaseChange::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase3),
			FOnDarkMageStagePhaseUpdate::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase3)) });

	DarkMageStagePhaseManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Ending,
	FDarkMageStagePhaseWrapper(FOnDarkMageStagePhaseChange::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangeEnding),
		FOnDarkMageStagePhaseUpdate::CreateUObject(this, &AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdateEnding)) });

	SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase1);

}

void AUSDFBossDarkMageGameStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DarkMageStagePhaseManager[CurrentDarkMageStagePhase].OnDarkMageStagePhaseUpdate.ExecuteIfBound(DeltaTime);
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangeIntro()
{
	
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase1()
{
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (Phase1SeqAsset == nullptr)
		return;
	
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Phase1SeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhase1SeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerCharacter->DisableInput(PlayerController);

	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase2()
{
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (Phase2SeqAsset == nullptr)
		return;
		
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Phase2SeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhase2SeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerCharacter->DisableInput(PlayerController);

	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase3()
{
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (Phase3SeqAsset == nullptr)
		return;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Phase3SeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhase3SeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerCharacter->DisableInput(PlayerController);

	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangeEnding()
{

}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdateIntro(float DeltaTime)
{
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase1(float DeltaTime)
{
	if (BossDarkMage->GetCurrentHealth() / BossDarkMage->GetMaxHealth() <= 0.5f)
	{
		SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase2);
		return;
	}
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase2(float DeltaTime)
{
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase3(float DeltaTime)
{
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdateEnding(float DeltaTime)
{
}

void AUSDFBossDarkMageGameStage::SetDarkMageStagePhase(EDarkMageStagePhase NewStagePhase)
{
	CurrentDarkMageStagePhase = NewStagePhase;
	DarkMageStagePhaseManager[CurrentDarkMageStagePhase].OnDarkMageStagePhaseChange.ExecuteIfBound();
}

void AUSDFBossDarkMageGameStage::OnPhase1SeqFinished()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerCharacter->EnableInput(PlayerController);

	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnBossEntry(BossDarkMage);
	}
}

void AUSDFBossDarkMageGameStage::OnPhase2SeqFinished()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerCharacter->EnableInput(PlayerController);
}

void AUSDFBossDarkMageGameStage::OnPhase3SeqFinished()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerCharacter->EnableInput(PlayerController);
}