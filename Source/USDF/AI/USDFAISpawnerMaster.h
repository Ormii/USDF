// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFAISpawnerMaster.generated.h"

UCLASS()
class USDF_API AUSDFAISpawnerMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFAISpawnerMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	void PrepareSpawnOrder(int32 StageNumber);
	void BeginSpawnOrder();

	// AISpawner Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AISpawner, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AUSDFAISpawner>> AISpawners;
};
