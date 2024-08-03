// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/USDFGameStageInterface.h"
#include "USDFDarkMageEyeCube.generated.h"

UCLASS()
class USDF_API AUSDFDarkMageEyeCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFDarkMageEyeCube();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> BaseEffect;

protected:
	UPROPERTY(VisibleAnywhere, Category = Spawner, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AUSDFDarkMageEyeCubeSpawner>> Spawners;

	UPROPERTY()
	FTimerHandle TimerHandle;

public:
	void PrepareSpawn(EGameStagePhase GameStagePhase);

protected:
	void Spawn();
};
