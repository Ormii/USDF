// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "USDF_AnimNotify_CheckHitPoint.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API UUSDF_AnimNotify_CheckHitPoint : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UUSDF_AnimNotify_CheckHitPoint();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
