// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/USDFGamestageInterface.h"
#include "USDFGameStage.generated.h"

DECLARE_DELEGATE(FOnGameStageChange)

UENUM(BlueprintType)
enum class EGameStageState : uint8
{
	Prepare,
	Battle,
	End
};

enum class EGameStagePlayerFOV : uint8
{
	EGameStagePlayerFOV_Default,
	EGameStagePlayerFOV_Wide,
};


USTRUCT()
struct FGameStageChangeWrapper
{
	GENERATED_BODY()

	FGameStageChangeWrapper() {}
	FGameStageChangeWrapper(const FOnGameStageChange& InOnGameStageChange) { OnGameStageChange = InOnGameStageChange; }

	FOnGameStageChange OnGameStageChange;
};


UCLASS()
class USDF_API AUSDFGameStage : public AActor, public IUSDFGameStageInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFGameStage();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetGameStage(EGameStagePhase NewGameStagePhase) {};
	virtual void SetPlayerFOV(EGameStagePlayerFOV NewGameStagePlayerFOV) {}

	EGameStagePlayerFOV GameStagePlayerFOV;

protected:
	virtual void BeginSequence();
	virtual void FinishSequence();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequencePlayer> SequencePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	class ALevelSequenceActor* LevelSequenceActor;

};
