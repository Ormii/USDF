// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "USDF_AnimNotifyState_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDF_AnimNotifyState_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UUSDF_AnimNotifyState_MeleeAttack();

protected:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
