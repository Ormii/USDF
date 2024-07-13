// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFAISpawner.generated.h"

USTRUCT(BlueprintType)
struct FStageSpawnOrder
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	int TargetStageNumber;

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	int SpawnMonsterCount;

	UPROPERTY(EditAnywhere, Category = StageSpawnOrder, Meta = (AllowPrivateAccess = "true"))
	float DelayTime;
};

UCLASS()
class USDF_API AUSDFAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// AISpawner Section
protected:
	UPROPERTY(VisibleAnywhere, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Point0;

	UPROPERTY(VisibleAnywhere, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Point1;

	UPROPERTY(VisibleAnywhere, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Point2;

	UPROPERTY(VisibleAnywhere, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Point3;

	UPROPERTY(VisibleAnywhere, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Point4;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFCharacterNonPlayer> SpawnMonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TArray<FStageSpawnOrder> TotalStageSpawnOrder;
	
	TQueue<FStageSpawnOrder> CurrentSpawnOrder;

	void Spawn(const FStageSpawnOrder& SpawnOrder);

	int32 StageNumber;
	bool bIsSpawnning;
	float SpawnningTime;

public:
	virtual void PrepareSpawn(int32 InStageNumber);
	virtual void BeginSpawn();

public:
	// Goal Section
	void SetGoal(TMap<UClass*, int>& Goal);
};
