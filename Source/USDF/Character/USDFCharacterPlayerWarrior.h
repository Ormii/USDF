// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterPlayer.h"
#include "Damage/USDFDamageCommon.h"
#include "USDFCharacterPlayerWarrior.generated.h"

class UUSDFComboActionData;

UENUM()
enum class EPlayerWarriorComboType
{
	None,
	Default,
	UpperCut,
	Dash,
	Power,
};


DECLARE_DELEGATE(FComboAttackDelegate);

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
	virtual void PostInitializeComponents()override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;


protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFItemWeapon> WeaponSwordClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AUSDFItemWeapon> WeaponSword;

// Input Section
public:
	virtual void Attack()override;
	virtual void ReleaseAttack()override;
	virtual void AttackQKey()override;
	virtual void AttackEKey()override;
	virtual void AttackRKey()override;
	virtual void Dodge()override;

	// AnimInterface Section
private:
	virtual void EquipWeapon();
	virtual void UnEquipWeapon();
	virtual void CheckApplyDamagePoint()override;
	virtual void AttackHitCheck() override;
	virtual bool IsCombatState() override;
	class UParticleSystem* GetTrailParticleSystem() override;
	FName GetTrailStartSocketName() override;
	FName GetTrailEndSocketName() override;
	
	// Animation Section
private:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CombatStartMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CombatEndMontage;

	UFUNCTION()
	void CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	bool PossessCombatStartMontage();
	void PossessCombatEndMontage();
	void PossessAttackMontage();

	// Combat Section
protected:
	UPROPERTY()
	TMap<EPlayerWarriorComboType, FComboAttackDelegateWrapper> ComboAttackDelegateManager;

	UPROPERTY()
	TMap<EPlayerWarriorComboType, TObjectPtr<UUSDFComboActionData>> ComboAttackDataManager;

	UPROPERTY(Transient, VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	uint8 bCombatState : 1;

	
	EPlayerWarriorComboType CurrentComboAttackType;
	int32					CurrentComboCount;
	bool IgnoreComboCommand;
	bool HasNextComboCommand;
	float CombatStateTime;
	FVector PowerAttackAreaLocation;


	virtual void CheckCombo()override;
	void ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetCombatState(bool NewCombatState);

	void DefaultComboAttack();
	void UpperAttack();
	void DashAttack();
	void PowerAttack();
	
	void DefaultAttackHitCheck();
	void UpperAttackHitCheck();
	void DashAttackHitCheck();

	void ApplyDamagePowerAttack();

	void ExecuteDefaultHitCheck();
	void ExecuteDashHitCheck();
	void ResetCombatStateTime();

	// Attack Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

	// Damage Section
protected:
	virtual void OnDamageResponse(FDamageInfo DamageInfo)override;
};
