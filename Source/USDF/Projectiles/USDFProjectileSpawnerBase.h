// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFProjectileSpawnerBase.generated.h"

UCLASS()
class USDF_API AUSDFProjectileSpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFProjectileSpawnerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SpawnProjectile();


	// Spawn Section
protected:
	UPROPERTY(VisibleAnywhere, Category = SpawnPosition, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class USceneComponent> SpawnPosition;

	UPROPERTY(VisibleAnywhere, Category = SpawnEffect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> SpawnEffect;

	UPROPERTY(Transient)
	float SpawnDelayTime;

	UPROPERTY(Transient)
	float DestroyDelayTime;

	FORCEINLINE void SetSpawnDelayTime(float NewSpawnDelayTime) { SpawnDelayTime = NewSpawnDelayTime; }

protected:
	UPROPERTY(EditAnywhere, Category = Projectile, Meta =(AllowPrivateAccess= "true"))
	TSubclassOf<class AUSDFProjectileBase> ProjectileClass;

	UPROPERTY(Transient)
	float ProjectileInitSpeed;

	UPROPERTY(Transient)
	float AttackDamage;

	UPROPERTY()
	FTimerHandle TimerHandle;

public:
	FORCEINLINE void SetAttackDamage(float NewAttackDamage) { AttackDamage = NewAttackDamage; }
};
