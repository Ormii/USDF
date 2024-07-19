// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Damage/USDFDamageCommon.h"
#include "USDFDamageSystemComponent.generated.h"


DECLARE_DELEGATE_OneParam(FOnDamageResponse, FDamageInfo /*DamageInfo*/)
DECLARE_DELEGATE(FOnDeath)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USDF_API UUSDFDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSDFDamageSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:
	void Heal(class UUSDFCharacterStatComponent* Stat, float HealAmount);
	void TakeDamage(class UUSDFCharacterStatComponent* Stat, FDamageInfo InDamageInfo);

public:
	FORCEINLINE bool GetIsDead() const { return bIsDead; }

public:
	FOnDamageResponse OnDamageResponse;
	FOnDeath OnDeath;

protected:
	UPROPERTY(VisibleAnywhere, Category = Damage, Meta = (AllowPrivateAccess="true"))
	uint8 bIsDead : 1;
};
