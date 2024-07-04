// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterPlayer.h"
#include "USDFCharacterPlayerWarrior.generated.h"

class UUSDFComboActionData;

UENUM()
enum class EPlayerWarriorComboType
{
	Default
};

DECLARE_DELEGATE_OneParam(FComboAttackDelegate, int /*NextCombo*/);

USTRUCT()
struct FComboAttackDelegateWrapper
{
	GENERATED_BODY();

	FComboAttackDelegateWrapper() {};
	FComboAttackDelegateWrapper(const FComboAttackDelegate& InComboAttackDelegate) 
		: OnComboAttackDelegate(InComboAttackDelegate){}

	FComboAttackDelegate OnComboAttackDelegate;
};

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
	virtual void PostInitializeComponents()override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WeaponStaticMesh;

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

	UFUNCTION()
	void CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	UFUNCTION()
	void CombatEndMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EquipWeapon() override;
	virtual void UnEquipWeapon() override;

	virtual void AttackHitCheck() override;
	UFUNCTION()
	void OnWarriorLanded(const FHitResult& Hit);

// Combat Section
protected:
	virtual bool CheckCombo() override;
	void DefaultComboAttack(int32 NextCombo);
	void ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UPROPERTY()
	TMap<EPlayerWarriorComboType, FComboAttackDelegateWrapper> ComboAttackDelegateManager;
	
	UPROPERTY()
	TMap<EPlayerWarriorComboType, TObjectPtr<UUSDFComboActionData>> ComboAttackDataManager;

	EPlayerWarriorComboType CurrentComboAttackType;
	int32					CurrentComboCount;

// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;
};
