// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterPlayer.h"
#include "USDFCharacterPlayerWarrior.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterPlayerWarrior : public AUSDFCharacterPlayer
{
	GENERATED_BODY()
	
public:
	AUSDFCharacterPlayerWarrior();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

// Combat Section
protected:

	float CombatStateTime;

	virtual void SetCombatState(bool NewCombatState) override;

// Input Section
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;
	virtual void Attack()override;

	void WarriorJump();
	void WarriorStopJumping();

// Animation Section
private:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CombatStartMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CombatEndMontage;

	void PossessCombatStartMontage();
	void PossessCombatEndMontage();

	void CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void CombatEndMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UFUNCTION()
	void OnWarriorLanded(const FHitResult& Hit);
};
