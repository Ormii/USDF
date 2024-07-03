// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USDFCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UUSDFCharacterControlData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;

	UPROPERTY(EditAnywhere, Category = PlayerController)
	float ViewPitchMin;
	UPROPERTY(EditAnywhere, Category = PlayerController)
	float ViewPitchMax;

	UPROPERTY(EditAnywhere, Category = PlayerController)
	float ViewYawMin;
	UPROPERTY(EditAnywhere, Category = PlayerController)
	float ViewYawMax;

	UPROPERTY(EditAnywhere, Category = PlayerController)
	float ViewRollMin;
	UPROPERTY(EditAnywhere, Category = PlayerController)
	float ViewRollMax;
};
