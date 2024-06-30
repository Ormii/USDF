// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "USDFCharacterBase.generated.h"

UCLASS()
class USDF_API AUSDFCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSDFCharacterBase();


	// CharacterControl Section
protected:
	virtual void SetCharacterControlData(const class UUSDFCharacterControlData* NewCharacterControlData);
};
