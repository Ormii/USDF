// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBossMonster.h"
#include "USDFCharacterBossDarkMage.generated.h"


UENUM()
enum class EDarkMageActionType : uint8
{
	None,
	DefaultAttack,
	Meteo,
	UpLaser,
	OrderSpawn,
	PushBack,
};


/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterBossDarkMage : public AUSDFCharacterBossMonster
{
	GENERATED_BODY()

public:
	AUSDFCharacterBossDarkMage();

protected:
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> FuryEffect;

public:
	void ActivateFuryEffect();
	void DeActivateFuryEffect();

	// AI Section
protected:
	virtual void ActionByAI(EAIActionType InAIAttackType) override;
	virtual void ActionFinished() override;

	// Animation Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TMap<EDarkMageActionType, TObjectPtr<class UAnimMontage>> ActionMontages;

	UFUNCTION()
	void ActionMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void AttackFire() override;
	virtual void SpawnProjectile() override;
	virtual void SpawnLaser(int32 InParam) override;
	virtual void PushBackAction()override;
	virtual void BuffAction() override;
	virtual void TeleportStart() override;
	virtual void TeleportEnd() override;

	// Combat Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	EDarkMageActionType CurrentActionType;

	// Damage Section
protected:
	virtual void OnDeath() override;
	virtual void OnDamageResponse(FDamageInfo DamageInfo) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFEnemyProjectile> DefaultAtkProjectileClass;

	UPROPERTY()
	TObjectPtr<class AUSDFEnemyProjectile> DefaultAtkProjectile;

	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFDarkMageMeteoSpawner> MeteoSpawnerClass;

	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFDarkMageUpLaser> UpLaserClass;

	UPROPERTY(VisibleAnywhere, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFDarkMageElectLaser> ElectLaserClass;


	// Teleport Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Teleport, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> TeleportEffect;

	float SaveMaxFlySpeed;
	float SaveMaxAcceleration;

	//EyeCube
protected:
	UPROPERTY(VisibleAnywhere, Category = Additional, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AUSDFDarkMageEyeCube> DarkMageEyeCube;
};


EDarkMageActionType ConvertAIActionType2DarkMageType(EAIActionType AIActionType);
EAIActionType ConvertDarkMageType2AIActionType(EDarkMageActionType DarkMageActionType);