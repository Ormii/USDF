// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/USDFTriggerBase.h"
#include "Interface/USDFGameStageInterface.h"
#include "USDFPhaseTrigger.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFPhaseTrigger : public AUSDFTriggerBase
{
	GENERATED_BODY()
	
public:
	AUSDFPhaseTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, Category = Phase, Meta = (AllowPrivateAccess = "true"))
	EGameStagePhase GameStagePhase;
};
