// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/USDFBossDarkMageGameStage.h"
#include "LevelSequencePlayer.h"
#include "Interface/USDFGameModeInterface.h"
#include "Interface/USDFDamageableInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Game/USDFGameMode.h"
#include "Player/USDFPlayerController.h"
#include "AI/USDFBossDarkMageAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/USDFCharacterPlayer.h"
#include "Character/USDFCharacterBossDarkMage.h"
#include "Character/USDFCharacterNormalMonster.h"
#include "Enemy/USDFDarkMageDotDamageZone.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Decal/USDFDarkMageDecal.h"
#include "LegacyCameraShake.h"
#include "Enemy/USDFDarkMageDotDamageZone.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"

AUSDFBossDarkMageGameStage::AUSDFBossDarkMageGameStage()
{
	// CDO
	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	
	/*
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

		static ConstructorHelpers::FObjectFinder<ULevelSequence> Phase3SeqAssetRef(TEXT("/Game/Cinematics/BossFinalPhase/BossFinalPhase.BossFinalPhase"));
		if (Phase3SeqAssetRef.Object)
		{
			Phase3SeqAsset = Phase3SeqAssetRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<ULevelSequence> PhaseEndingSeqAssetRef(TEXT("/Game/Cinematics/EndingPhase/EndingPhase.EndingPhase"));
		if (PhaseEndingSeqAssetRef.Object)
		{
			PhaseEndingSeqAsset = PhaseEndingSeqAssetRef.Object;
		}
	
	
	*/

	static ConstructorHelpers::FObjectFinder<USoundCue> IntroSoundCueRef(TEXT("/Game/ReferenceAsset/cplomedia_CinematicVol5/Cues/Cue_Lost_Cue.Cue_Lost_Cue"));
	if(IntroSoundCueRef.Object)
	{
		DarkMageBGMManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Intro, IntroSoundCueRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Phase1SoundCueRef(TEXT("/Game/ReferenceAsset/cplomedia_CinematicVol5/Cues/Cue_Hero_Cue.Cue_Hero_Cue"));
	if (Phase1SoundCueRef.Object)
	{
		DarkMageBGMManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Phase1, Phase1SoundCueRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Phase2SoundCueRef(TEXT("/Game/ReferenceAsset/cplomedia_CinematicVol5/Cues/Cue_Allin_Cue.Cue_Allin_Cue"));
	if (Phase2SoundCueRef.Object)
	{
		DarkMageBGMManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Phase2, Phase2SoundCueRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> Phase3SoundCueRef(TEXT("/Game/ReferenceAsset/cplomedia_CinematicVol5/Cues/Cue_Freedom_Cue.Cue_Freedom_Cue"));
	if (Phase3SoundCueRef.Object)
	{
		DarkMageBGMManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Phase3, Phase3SoundCueRef.Object });
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> EndingSoundCueRef(TEXT("/Game/ReferenceAsset/cplomedia_CinematicVol5/Cues/Cue_Wind_Cue.Cue_Wind_Cue"));
	if (EndingSoundCueRef.Object)
	{
		DarkMageBGMManager.Add({ EDarkMageStagePhase::EDarkMageStagePhase_Ending, EndingSoundCueRef.Object });
	}

	static ConstructorHelpers::FClassFinder<AUSDFDarkMageDotDamageZone> DarkMageDotDamageZoneClassRef(TEXT("/Game/Blueprint/Enemy/BP_USDFDarkMageDotDamageZone.BP_USDFDarkMageDotDamageZone_C"));
	if (DarkMageDotDamageZoneClassRef.Class)
	{
		DarkMageDotDamageZoneClass = DarkMageDotDamageZoneClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<ULegacyCameraShake> DotDamageZoneCameraShakeClassRef(TEXT("/Game/Blueprint/Camera/BP_DotDamageZoneCameraShake.BP_DotDamageZoneCameraShake_C"));
	if (DotDamageZoneCameraShakeClassRef.Class)
	{
		DotDamageZoneCameraShakeClass = DotDamageZoneCameraShakeClassRef.Class;
	}

	CurrentDarkMageStagePhase = EDarkMageStagePhase::EDarkMageStagePhase_Intro;
	DotDamageZoneCurrentTag = 0;
	bSceneChanging = false;

	DefaultPlayerFOV = 90.0f;
	WidePlayerFOV = 120.0f;
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

	AActor* Decal = UGameplayStatics::GetActorOfClass(this, AUSDFDarkMageDecal::StaticClass());
	DarkMageDecal = Cast<AUSDFDarkMageDecal>(Decal);
	check(DarkMageDecal);

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

	BossDarkMage->OnBossMonsterActionStart.BindUObject(this, &AUSDFBossDarkMageGameStage::SetGameStageBossActionStart);
	BossDarkMage->OnBossMonsterActionEnd.BindUObject(this, &AUSDFBossDarkMageGameStage::SetGameStageBossActionEnd);

	SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Intro);

	DarkMageDecal->SetActorHiddenInGame(true);
}

void AUSDFBossDarkMageGameStage::BeginDestroy()
{
	UWorld* World = GetWorld();
	if (TimerHandle.IsValid() && World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}


	Super::BeginDestroy();
}

void AUSDFBossDarkMageGameStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCameraComponent* Camera = PlayerCharacter->GetCameraComponent();
	if (Camera)
	{
		switch (GameStagePlayerFOV)
		{
			case EGameStagePlayerFOV::EGameStagePlayerFOV_Default:
				Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, DefaultPlayerFOV, DeltaTime, 0.7f);
				break;
			case EGameStagePlayerFOV::EGameStagePlayerFOV_Wide:
				Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, WidePlayerFOV, DeltaTime, 0.7f);
				break;
			default:
				break;
		}
	}

	DarkMageStagePhaseManager[CurrentDarkMageStagePhase].OnDarkMageStagePhaseUpdate.ExecuteIfBound(DeltaTime);
}

void AUSDFBossDarkMageGameStage::SetGameStage(EGameStagePhase NewGameStagePhase)
{
	switch (NewGameStagePhase)
	{
		case EGameStagePhase::EGameStagePhase_Intro:
			SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Intro);
			break;
		case EGameStagePhase::EGameStagePhase_Phase1:
			SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase1);
			break;

		case EGameStagePhase::EGameStagePhase_Phase2:
			SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase2);
			break;

		case EGameStagePhase::EGameStagePhase_Phase3:
			SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase3);
			break;

		case EGameStagePhase::EGameStagePhase_Ending:
			SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Ending);
			break;

		default:
			break;
	}
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangeIntro()
{
	if (Audio)
		Audio->Stop();

	Audio = UGameplayStatics::SpawnSound2D(this, DarkMageBGMManager[EDarkMageStagePhase::EDarkMageStagePhase_Intro]);

	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnGameStageIntro();
	}
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase1()
{
	bSceneChanging = true;
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (Phase1SeqAsset == nullptr)
	{
		bSceneChanging = false;
		OnPhase1SeqFinished();
		return;
	}
	
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Phase1SeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhase1SeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	BeginSequence();

	
	if (Audio)
		Audio->Stop();

	Audio = UGameplayStatics::SpawnSound2D(this, DarkMageBGMManager[EDarkMageStagePhase::EDarkMageStagePhase_Phase1]);
	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase2()
{
	bSceneChanging = true;
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (Phase2SeqAsset == nullptr)
	{
		bSceneChanging = false;
		OnPhase2SeqFinished();
		return;
	}
		
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Phase2SeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhase2SeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	BeginSequence();

	if (Audio)
		Audio->Stop();

	Audio = UGameplayStatics::SpawnSound2D(this, DarkMageBGMManager[EDarkMageStagePhase::EDarkMageStagePhase_Phase2]);
	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangePhase3()
{
	bSceneChanging = true;
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (Phase3SeqAsset == nullptr)
	{
		bSceneChanging = false;
		OnPhase3SeqFinished();
		return;
	}

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Phase3SeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhase3SeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	BeginSequence();

	if (Audio)
		Audio->Stop();

	Audio = UGameplayStatics::SpawnSound2D(this, DarkMageBGMManager[EDarkMageStagePhase::EDarkMageStagePhase_Phase3]);
	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_ChangeEnding()
{
	bSceneChanging = true;
	if (SequencePlayer != nullptr && SequencePlayer->IsPlaying())
		SequencePlayer->Stop();

	if (PhaseEndingSeqAsset == nullptr)
	{
		bSceneChanging = false;
		OnPhaseEndingSeqFinished();
		return;
	}

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), PhaseEndingSeqAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, "OnPhaseEndingSeqFinished");
	SequencePlayer->OnFinished.AddUnique(ScriptDelegate);

	PlayerCharacter->SetBoneLayeredBlendEnable(false);
	BossDarkMage->DeActivateFuryEffect();
	
	BeginSequence();

	TArray<AActor*> NormalMonsters;
	UGameplayStatics::GetAllActorsOfClass(this, AUSDFCharacterNormalMonster::StaticClass(), NormalMonsters);
	for (int32 i = 0; i < NormalMonsters.Num(); ++i)
	{
		IUSDFDamageableInterface* Damageable = Cast<IUSDFDamageableInterface>(NormalMonsters[i]);
		if (Damageable == nullptr)
			continue;
		
		FDamageInfo DamageInfo{};
		DamageInfo.DamageAmount = 100000;
		DamageInfo.DamageCauser = PlayerCharacter;
		DamageInfo.DamageType = EDamageType::HitDefault;
		Damageable->TakeDamage(DamageInfo);
	}

	if (Audio)
		Audio->Stop();

	Audio = UGameplayStatics::SpawnSound2D(this, DarkMageBGMManager[EDarkMageStagePhase::EDarkMageStagePhase_Ending]);
	SequencePlayer->Play();
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdateIntro(float DeltaTime)
{
	
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase1(float DeltaTime)
{
	if (bSceneChanging)
		return;

	if (BossDarkMage->GetCurrentHealth() / BossDarkMage->GetMaxHealth() <= 0.7f)
	{
		SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase2);
		return;
	}
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase2(float DeltaTime)
{
	if (bSceneChanging)
		return;

	if (BossDarkMage->GetCurrentHealth() / BossDarkMage->GetMaxHealth() <= 0.4f)
	{
		SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Phase3);
		return;
	}
}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdatePhase3(float DeltaTime)
{
	if (bSceneChanging)
		return;

	if (BossDarkMage->GetCurrentHealth() / BossDarkMage->GetMaxHealth() <= 0.0f)
	{
		SetDarkMageStagePhase(EDarkMageStagePhase::EDarkMageStagePhase_Ending);
		return;
	}

	DropDotDamageZoneTime -= DeltaTime;

	if (DropDotDamageZoneTime <= 1000.0f)
	{
		DarkMageDecal->SetActorHiddenInGame(false);
		DarkMageDecal->SetActorLocation(PlayerCharacter->GetActorLocation());
		if (DropDotDamageZoneTime <= 0.0f)
		{
			AUSDFDarkMageDotDamageZone* DarkMageDotDamageZone = GetWorld()->SpawnActorDeferred<AUSDFDarkMageDotDamageZone>(DarkMageDotDamageZoneClass, FTransform::Identity);
			if (DarkMageDotDamageZone != nullptr)
			{
				FTransform Transform = FTransform::Identity;
				Transform.SetLocation(PlayerCharacter->GetActorLocation() + FVector::UpVector * 3000.0f);
				DarkMageDotDamageZone->OnHitEventAccur.BindUFunction(this, "HitCameraShake");
				DarkMageDotDamageZone->FinishSpawning(Transform);
			}
			DarkMageDecal->SetActorHiddenInGame(true);
			DropDotDamageZoneTime = FMath::RandRange(2000.0f, 3000.0f);
		}
	}

}

void AUSDFBossDarkMageGameStage::DarkMageStagePhase_UpdateEnding(float DeltaTime)
{
	if (bSceneChanging)
		return;
}

void AUSDFBossDarkMageGameStage::SetDarkMageStagePhase(EDarkMageStagePhase NewStagePhase)
{
	CurrentDarkMageStagePhase = NewStagePhase;
	DarkMageStagePhaseManager[CurrentDarkMageStagePhase].OnDarkMageStagePhaseChange.ExecuteIfBound();
}

void AUSDFBossDarkMageGameStage::OnPhase1SeqFinished()
{
	FinishSequence();
	bSceneChanging = false;

	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnBossEntry(BossDarkMage);
	}
}

void AUSDFBossDarkMageGameStage::OnPhase2SeqFinished()
{

	FinishSequence();
	bSceneChanging = false;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AUSDFBossDarkMageGameStage::SetDarkMageAINormalMonsterCount);

	if(GetWorld())
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 10.0f, true);

	AUSDFAIController* AIController = Cast<AUSDFAIController>(BossDarkMage->GetController());
	if (AIController)
	{
		AIController->SetCurrentPhase(EGameStagePhase::EGameStagePhase_Phase2);
	}

}

void AUSDFBossDarkMageGameStage::OnPhase3SeqFinished()
{
	FinishSequence();
	bSceneChanging = false;

	AUSDFAIController* AIController = Cast<AUSDFAIController>(BossDarkMage->GetController());
	if (AIController)
	{
		AIController->SetCurrentPhase(EGameStagePhase::EGameStagePhase_Phase3);
	}

	BossDarkMage->ActivateFuryEffect();

	DropDotDamageZoneTime = FMath::RandRange(1000.0f, 2000.0f);
}

void AUSDFBossDarkMageGameStage::OnPhaseEndingSeqFinished()
{
	FinishSequence();
	PlayerCharacter->SetBoneLayeredBlendEnable(true);

	bSceneChanging = false;

	TArray<AActor*> NormalMonsters;
	UGameplayStatics::GetAllActorsOfClass(this, AUSDFCharacterNormalMonster::StaticClass(), NormalMonsters);
	for (int32 i = 0; i < NormalMonsters.Num(); ++i)
	{
		AUSDFCharacterNormalMonster* NormalMonster = Cast<AUSDFCharacterNormalMonster>(NormalMonsters[i]);
		if (NormalMonster)
		{
			NormalMonster->Destroy();
		}
	}

	TArray<AActor*> DamageZones;
	UGameplayStatics::GetAllActorsOfClass(this, AUSDFDarkMageDotDamageZone::StaticClass(), DamageZones);
	for (int32 i = 0; i < DamageZones.Num(); ++i)
	{
		AUSDFDarkMageDotDamageZone* DamageZone = Cast<AUSDFDarkMageDotDamageZone>(DamageZones[i]);
		if (DamageZone)
		{
			DamageZone->Destroy();
		}
	}

	IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnBossDead(BossDarkMage);
		GameModeInterface->OnGameStageEnding();
	}

	DarkMageDecal->SetActorHiddenInGame(true);
}

void AUSDFBossDarkMageGameStage::BeginSequence()
{
	Super::BeginSequence();

	AUSDFPlayerController* PlayerController = CastChecked<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerController->BeginSequence();
	PlayerCharacter->DisableInput(PlayerController);
	/*
		AUSDFAIController* AIController = Cast<AUSDFAIController>(BossDarkMage->GetController());
		if (AIController)
		{
			AIController->StopAI();
		}
	*/
}

void AUSDFBossDarkMageGameStage::FinishSequence()
{
	Super::FinishSequence();

	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerController->FinishSequence();
	PlayerCharacter->EnableInput(PlayerController);
	/*
		AUSDFAIController* AIController = Cast<AUSDFAIController>(BossDarkMage->GetController());
		if (AIController)
		{
			AIController->RunAI();
		}
	*/
}

void AUSDFBossDarkMageGameStage::HitCameraShake()
{
	AUSDFPlayerController* PlayerController = Cast<AUSDFPlayerController>(PlayerCharacter->GetController());
	PlayerController->ClientStartCameraShake(DotDamageZoneCameraShakeClass);
}

void AUSDFBossDarkMageGameStage::SetPlayerFOV(EGameStagePlayerFOV NewGameStagePlayerFOV)
{
	GameStagePlayerFOV = NewGameStagePlayerFOV;
}

void AUSDFBossDarkMageGameStage::SetGameStageBossActionStart(EAIActionType AIActionType)
{
	EDarkMageActionType DarkMageActionType = ConvertAIActionType2DarkMageType(AIActionType);
	switch (DarkMageActionType)
	{
		case EDarkMageActionType::Meteo:
			SetPlayerFOV(EGameStagePlayerFOV::EGameStagePlayerFOV_Wide);
			break;
		default:
			break;
	}
}

void AUSDFBossDarkMageGameStage::SetGameStageBossActionEnd(EAIActionType AIActionType)
{
	SetPlayerFOV(EGameStagePlayerFOV::EGameStagePlayerFOV_Default);
}

void AUSDFBossDarkMageGameStage::SetDarkMageAINormalMonsterCount()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	AUSDFBossDarkMageAIController* DarkMageAIController = Cast<AUSDFBossDarkMageAIController>(BossDarkMage->GetController());
	if (DarkMageAIController == nullptr)
		return;

	TArray<AActor*> NormalMonsters;
	UGameplayStatics::GetAllActorsOfClass(World, AUSDFCharacterNormalMonster::StaticClass(), NormalMonsters);
	DarkMageAIController->SetNormalMonsterCount(NormalMonsters.Num());
}
