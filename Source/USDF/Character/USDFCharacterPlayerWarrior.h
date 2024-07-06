// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterPlayer.h"
#include "Interface/USDFCharacterHitReactInterface.h"
#include "USDFCharacterPlayerWarrior.generated.h"

class UUSDFComboActionData;

UENUM()
enum class EPlayerWarriorComboType
{
	None,
	Default,
	UpperCut,
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
class USDF_API AUSDFCharacterPlayerWarrior : public AUSDFCharacterPlayer, public IUSDFCharacterHitReactInterface
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

// Input Section
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;
	virtual void Attack()override;
	virtual void ReleaseAttack()override;

	void WarriorJump();
	void WarriorStopJumping();

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

	virtual void AttackHitCheck() override;
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


	EPlayerWarriorComboType CurrentComboAttackType;
	int32					CurrentComboCount;

	virtual bool IsCombatState() override;
	virtual void CheckCombo()override;
	void ComboActionEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetCombatState(bool NewCombatState);

	void DefaultComboAttack();
	void UpperAttack();
	
	void DefaultAttackHitCheck();
	void UpperAttackHitCheck();

// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, Category = AttackHit, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UNiagaraSystem>> AttackHitEffects;

// Hit React Section
protected:
	bool bHitReactState;

	virtual void HitReact(const FHitResult& HitResult, const float DamageAmount, EHitReactType HitReactType, const AActor* HitCauser) override;
	virtual bool GetHitReactState() override;
};
