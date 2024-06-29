// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USDFCharacterBase.h"
#include "InputActionValue.h"
#include "USDFCharacterPlayer.generated.h"

UENUM()
enum class ECharacterPlayerControlType : uint8
{
	Shoulder
};

/**
 * 
 */
UCLASS()
class USDF_API AUSDFCharacterPlayer : public AUSDFCharacterBase
{
	GENERATED_BODY()
public:
	AUSDFCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

// CharacterControl Section
protected:
	void SetCharacterControl(ECharacterPlayerControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UUSDFCharacterControlData* NewCharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterPlayerControlType, class UUSDFCharacterControlData*> CharacterControlManager;

// Input Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	ECharacterPlayerControlType CurrentControlType;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

};
