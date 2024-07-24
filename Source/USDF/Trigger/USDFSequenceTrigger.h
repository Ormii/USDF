// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/USDFTriggerBase.h"
#include "USDFSequenceTrigger.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFSequenceTrigger : public AUSDFTriggerBase
{
	GENERATED_BODY()

public:
	AUSDFSequenceTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequencePlayer> SequencePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULevelSequence> SequenceAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sequence, Meta = (AllowPrivateAccess = "true"))
	class ALevelSequenceActor* LevelSequenceActor;

protected:
	UPROPERTY()
	TObjectPtr<class APlayerController> TriggeredPlayerController;

	UFUNCTION()
	void OnOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSequenceFinished();
};
