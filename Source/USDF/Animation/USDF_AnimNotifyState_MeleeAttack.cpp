// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDF_AnimNotifyState_MeleeAttack.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/USDFCollision.h"
#include "DrawDebugHelpers.h"

UUSDF_AnimNotifyState_MeleeAttack::UUSDF_AnimNotifyState_MeleeAttack()
{
}

void UUSDF_AnimNotifyState_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	IUSDFCharacterAttackAnimInterface* AttackCharacter = Cast<IUSDFCharacterAttackAnimInterface>(MeshComp->GetOwner());

	if (AttackCharacter)
	{
		AttackCharacter->AttackHitCheck();
	}
}
