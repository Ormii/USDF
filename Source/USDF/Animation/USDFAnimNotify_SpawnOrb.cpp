// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFAnimNotify_SpawnOrb.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"

void UUSDFAnimNotify_SpawnOrb::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IUSDFCharacterAttackAnimInterface* AttackAnim = Cast<IUSDFCharacterAttackAnimInterface>(MeshComp->GetOwner());
	if (AttackAnim)
	{
		AttackAnim->SpawnOrb();
	}
}
