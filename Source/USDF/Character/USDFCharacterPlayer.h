// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"
#include "Interface/USDFCharacterPlayerHUDInterface.h"
#include "USDFCharacterPlayer.generated.h"

UENUM()
enum class ECharacterPlayerControlType : uint8
{
	Shoulder,
	Preview,
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterPlayer : public AUSDFCharacterBase, public IUSDFCharacterPlayerAnimInterface
	,public IUSDFCharacterPlayerHUDInterface
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

	UPROPERTY(VisibleAnywhere, Category = Detect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> DetectSphere;

// Dead Section
protected:
	virtual void SetDead() override;

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
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ViewChangeAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	ECharacterPlayerControlType CurrentControlType;

	void Move(const FInputActionValue& Value);
	void ReleaseMove(const FInputActionValue& value);
	void Look(const FInputActionValue& Value);
	void PressViewChange();
	void ReleaseViweChange();
	void Sprint();
	void StopSprint();
	virtual void Attack();
	virtual void ReleaseAttack();

	bool bAttackKeyPress;
	float AttackKeyPressTime;

// Character Movement Section
protected:
	UPROPERTY(Transient, VisibleAnywhere, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	uint8 bSprintKeyPress : 1;

	FVector2D MovementInputValue;

// Animation Section
public:
	virtual bool IsSprintState() override;
	virtual FVector2D GetMovementInputValue() override;

	virtual bool IsAttackState() override;
	virtual bool IsDeadState() override;

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

// Attack Hit Section
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
