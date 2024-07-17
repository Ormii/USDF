// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/USDFAICommon.h"
#include "USDFAIController.generated.h"


struct FAIStimulus;
/**
 * 
 */
UCLASS()
class USDF_API AUSDFAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AUSDFAIController();

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	bool CanSenseActor(AActor* InActor, EAISense AISense, FAIStimulus& OutStimulus);
	void HandleSensedSight(AActor* InActor);
	void HandleSensedSound(FVector Location);
	void HandleSensedDamage(AActor* InActor);

	void SetCurrentAIState(EAIState NewState, FAISensedParam Param);
	EAIState GetCurrentAIState();

public:
	void RunAI();
	void StopAI();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Damage> DamageConfig;

protected:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> AttackTarget;
};
