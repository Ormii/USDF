// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USDFCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USDF_API UUSDFCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSDFCharacterStatComponent();

protected:

	virtual void InitializeComponent() override;

	// HP Section
public:
	FOnHpZeroDelegate OnHpZeroDelegate;
	FOnHpChangedDelegate OnHpChangedDelegate;

	FORCEINLINE const float GetMaxHp()const { return MaxHp; }
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	FORCEINLINE const float GetCurrentHp() const { return CurrentHp; }
	
	void SetCurrentHp(float NewCurrentHp);

	void ApplyDamage(float DamageAmount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxHp;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess= "true"))
	float CurrentHp;
};
