// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/USDFGameStageInterface.h"
#include "USDFDarkMageEyeCubeSpawner.generated.h"

USTRUCT(BlueprintType)
struct FDarkMageEyeCubeSpawnOrder
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	EGameStagePhase SpawnFlag;

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	float DelayTime;
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageEyeCubeSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageEyeCubeSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> SpawnEffect;

public:
	void PrepareSpawn(EGameStagePhase GameStagePhase);
	void BeginSpawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFCharacterNonPlayer> SpawnMonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TArray<FDarkMageEyeCubeSpawnOrder> TotalSpawnOrder;

	TQueue<FDarkMageEyeCubeSpawnOrder> CurrentSpawnOrder;

	virtual void Spawn(const FDarkMageEyeCubeSpawnOrder& SpawnOrder);

	EGameStagePhase SpawnFlag;
	bool bIsSpawnning;
	float SpawnningTime;
};