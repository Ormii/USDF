// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFAnimNotify_PushBackAction.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"

void UUSDFAnimNotify_PushBackAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IUSDFCharacterAttackAnimInterface* AttackAnim = Cast<IUSDFCharacterAttackAnimInterface>(MeshComp->GetOwner());
	if (AttackAnim)
	{
		AttackAnim->PushBackAction();
	}
}
