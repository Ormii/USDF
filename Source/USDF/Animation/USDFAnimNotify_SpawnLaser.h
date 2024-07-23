// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "USDFAnimNotify_SpawnLaser.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDFAnimNotify_SpawnLaser : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Param, Meta = (AllowPrivateAccess = "true"))
	int32 Param;
};
