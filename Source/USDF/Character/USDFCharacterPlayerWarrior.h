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
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents()override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUSDFItemWeapon> WeaponSwordClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AUSDFItemWeapon> WeaponSword;

// Input Section
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;
	virtual void Attack()override;
	virtual void ReleaseAttack()override;

	virtual void AttackQKey()override;
	virtual void AttackEKey()override;
	virtual void AttackRKey()override;

	virtual void Dodge()override;
	virtual void StopDodge()override;

// Animation Section
private:
	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CombatStartMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> CombatEndMontage;

	bool PossessCombatStartMontage();
	void PossessCombatEndMontage();

	void PossessAttackMontage();

	UFUNCTION()
	void CombatStartMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EquipWeapon();
	virtual void UnEquipWeapon();
	virtual void CheckApplyDamagePoint()override;

	virtual void AttackHitCheck() override;

	class UParticleSystem* GetTrailParticleSystem() override;
	FName GetTrailStartSocketName() override;
	FName GetTrailEndSocketName() override;

	UFUNCTION()
	void OnWarriorLanded(const FHitResult& Hit);

// Combat Section
protected:

	UPROPERTY(Transient, VisibleAnywhere, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	uint8 bCombatState : 1;

	UPROPERTY()
	TMap<EPlayerWarriorComboType, FComboAttackDelegateWrapper> ComboAttackDelegateManager;

	UPROPERTY()
	TMap<EPlayerWarriorComboType, TObjectPtr<UUSDFComboActionData>> ComboAttackDataManager;

	bool IgnoreComboCommand;
	bool HasNextComboCommand;
	float CombatStateTime;
	float UpperHitStateTime;
	bool bUpperHit;

	FVector PowerAttackAreaLocation;


	EPlayerWarriorComboType CurrentComboAttackType;
	int32					CurrentComboCount;

	virtual bool IsCombatState() override;
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

// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

// DamageSection
protected:
	virtual void OnDamageResponse(FDamageInfo DamageInfo)override;
};
