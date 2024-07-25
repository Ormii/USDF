// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"
#include "Interface/USDFCharacterPlayerHUDInterface.h"
#include "Interface/USDFCharacterPlayerInterface.h"
#include "Interface/USDFDamageableInterface.h"
#include "Damage/USDFDamageCommon.h"
#include "USDFCharacterPlayer.generated.h"

UENUM()
enum class ECharacterPlayerControlType : uint8
{
	Preview,
};

UENUM()
enum class EPlayerDodgeDirection
{
	None,
	Front,
	Front_Right,
	Front_Left,
	Back,
	Back_Right,
	Back_Left,
	Right,
	Left,
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterPlayer : public AUSDFCharacterBase, public IUSDFCharacterPlayerAnimInterface
	,public IUSDFCharacterPlayerHUDInterface, public IUSDFCharacterPlayerInterface, public IUSDFDamageableInterface
{
	GENERATED_BODY()
public:
	AUSDFCharacterPlayer();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;


// CharacterControl Section
protected:
	void SetCharacterControl(ECharacterPlayerControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UUSDFCharacterControlData* NewCharacterControlData) override;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterPlayerControlType, class UUSDFCharacterControlData*> CharacterControlManager;

	FRotator SpringArmRativeRotation;

// Input Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DodgeAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackQKeyAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackEKeyAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackRKeyAction;

	ECharacterPlayerControlType CurrentControlType;

	void Move(const FInputActionValue& Value);
	void ReleaseMove(const FInputActionValue& value);
	void Look(const FInputActionValue& Value);
	virtual void Dodge();
	virtual void StopDodge();
	virtual void Attack();
	virtual void ReleaseAttack();
	virtual void AttackQKey();
	virtual void ReleaseAttackQKey();
	virtual void AttackEKey();
	virtual void ReleaseAttackEKey();
	virtual void AttackRKey();
	virtual void ReleaseAttackRKey();

	bool bAttackKeyPress;
	bool bAttackQKeyPress;
	bool bAttackEKeyPress;
	bool bAttackRKeyPress;

	//Player Section
protected:
	virtual APlayerController* GetPlayerController() override;

// Character Movement Section
protected:
	UPROPERTY(Transient, VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	uint8 bDodgeKeyPress : 1;

	FVector2D MovementInputValue;

// Animation Section
public:
	virtual FVector2D GetMovementInputValue() override;

	virtual bool IsAttackState() override;
	virtual bool IsDeadState() override;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> DodgeLeft;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> DodgeRight;

	UPROPERTY()
	TMap<EPlayerDodgeDirection, TObjectPtr<class UAnimMontage>> DodgeAnimMontage;

protected:
	UPROPERTY(Transient, VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	uint8 bAttackState : 1;

// HUD Section
public:
	virtual void SetupPlayerHpBarHUDWidget(class UUSDFPlayerHpBarWidget* HpBar) override;


// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUSDFPlayerStatComponent> Stat;

	//Damage Section
protected:
	virtual float GetCurrentHealth() override;
	virtual float GetMaxHealth() override;
	virtual void Heal(float HealAmount) override;
	virtual void TakeDamage(FDamageInfo) override;
	virtual bool IsDead() override;
	virtual void OnDeath() override;


	// HitReact Section
protected:
	UPROPERTY()
	TMap<EHitReactDirection, TObjectPtr<class UAnimMontage>> HitReactAnimMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAnimMontage> HitReactKnockBackMontage;

	UPROPERTY()
	uint8 bDamagedState : 1;

	UFUNCTION()
	void OnHitReactMontageBlendOut(UAnimMontage* TargetMontage, bool bInterrupted);
	virtual void OnDamageResponse(FDamageInfo DamageInfo) override;
};
