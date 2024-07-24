// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/USDFGameStage.h"
#include "USDFBossDarkMageGameStage.generated.h"

DECLARE_DELEGATE(FOnDarkMageStagePhaseChange)
DECLARE_DELEGATE_OneParam(FOnDarkMageStagePhaseUpdate, float /*DeltaTime*/)

UENUM()
enum class EDarkMageStagePhase
{
	EDarkMageStagePhase_Intro,
	EDarkMageStagePhase_Phase1,
	EDarkMageStagePhase_Phase2,
	EDarkMageStagePhase_Phase3,
	EDarkMageStagePhase_Ending,
};

USTRUCT()
struct FDarkMageStagePhaseWrapper
{
	GENERATED_BODY()

	FDarkMageStagePhaseWrapper() {}
	FDarkMageStagePhaseWrapper(FOnDarkMageStagePhaseChange NewOnDarkMageStagePhaseChange,
		FOnDarkMageStagePhaseUpdate NewOnDarkMageStagePhaseUpdate) 
	{
		OnDarkMageStagePhaseChange = NewOnDarkMageStagePhaseChange;
		OnDarkMageStagePhaseUpdate = NewOnDarkMageStagePhaseUpdate;
	}


	FOnDarkMageStagePhaseChange OnDarkMageStagePhaseChange;
	FOnDarkMageStagePhaseUpdate OnDarkMageStagePhaseUpdate;
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFBossDarkMageGameStage : public AUSDFGameStage
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AUSDFBossDarkMageGameStage();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, Category = Characters, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AUSDFCharacterPlayer> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category = Characters, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AUSDFCharacterBossDarkMage> BossDarkMage;

protected:
	UPROPERTY(VisibleAnywhere, Category = Characters, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AUSDFCharacterNormalMonster>> NormalMonsters;

protected:
	UPROPERTY(VisibleAnywhere, Category = StagePhase, Meta = (AllowPrivateAccess = "true"))
	EDarkMageStagePhase CurrentDarkMageStagePhase;


protected:
	UPROPERTY()
	TMap<EDarkMageStagePhase, FDarkMageStagePhaseWrapper> DarkMageStagePhaseManager;


	void DarkMageStagePhase_ChangeIntro();
	void DarkMageStagePhase_ChangePhase1();
	void DarkMageStagePhase_ChangePhase2();
	void DarkMageStagePhase_ChangePhase3();
	void DarkMageStagePhase_ChangeEnding();

	void DarkMageStagePhase_UpdateIntro(float DeltaTime);
	void DarkMageStagePhase_UpdatePhase1(float DeltaTime);
	void DarkMageStagePhase_UpdatePhase2(float DeltaTime);
	void DarkMageStagePhase_UpdatePhase3(float DeltaTime);
	void DarkMageStagePhase_UpdateEnding(float DeltaTime);

public:
	void SetDarkMageStagePhase(EDarkMageStagePhase NewStagePhase);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequence> Phase1SeqAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequence> Phase2SeqAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequence> Phase3SeqAsset;

	UFUNCTION()
	void OnPhase1SeqFinished();
	
	UFUNCTION()
	void OnPhase2SeqFinished();
	
	UFUNCTION()
	void OnPhase3SeqFinished();
};
