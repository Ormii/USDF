// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/USDFDamageSystemComponent.h"
#include "CharacterStat/USDFCharacterStatComponent.h"

// Sets default values for this component's properties
UUSDFDamageSystemComponent::UUSDFDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUSDFDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUSDFDamageSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUSDFDamageSystemComponent::Heal(UUSDFCharacterStatComponent* Stat, float HealAmount)
{
	if (Stat == nullptr)
		return;

	Stat->SetCurrentHp(Stat->GetCurrentHp() + HealAmount);
}

void UUSDFDamageSystemComponent::TakeDamage(class UUSDFCharacterStatComponent* Stat, FDamageInfo InDamageInfo)
{
	if (Stat == nullptr)
		return;

	Stat->ApplyDamage(InDamageInfo.DamageAmount);

	OnDamageResponse.ExecuteIfBound(InDamageInfo);
	if (Stat->GetCurrentHp() <= 0)
	{
		bIsDead = true;
		OnDeath.ExecuteIfBound();
	}
}

