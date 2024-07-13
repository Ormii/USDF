// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFGameStage.generated.h"

DECLARE_DELEGATE(FOnGameStageChange)

UENUM(BlueprintType)
enum class EGameStageState : uint8
{
	Prepare,
	Battle,
	End
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
class USDF_API AUSDFGameStage : public AActor
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

	// GameStage Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameStage, Meta = (AllowPrivateAccess = "true"))
	EGameStageState CurrentGameStage;

	UPROPERTY(VisibleAnywhere, Category = GameGoal, Meta = (AllowPrivateAccess = "true"))
	TMap<EGameStageState, FGameStageChangeWrapper> GameStageChangeManager;

	int CurrentStageNumber;

	UPROPERTY(EditAnywhere, Category = StageNumber, Meta = (AllowPrivateAccess= "true"))
	int MaxStageNumber;



	void PrepareGameStage();
	void BattleGameStage();
	void EndGameStage();
	void CompleteGoal();

public:
	void SetGameStageState(EGameStageState NewGameStageState);
	FORCEINLINE int GetCurrentStageNumber() const { return CurrentStageNumber; }
	FORCEINLINE int GetMaxStageNumber() const { return MaxStageNumber; }
	// AISpawner Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AUSDFAISpawner>> AISpawners;

};
