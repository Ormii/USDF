// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDF_AnimNotify_Fire.h"
#include "USDF_AnimNotify_Fire.h"
#include "Interface/USDFCharacterAttackAnimInterface.h"

void UUSDF_AnimNotify_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IUSDFCharacterAttackAnimInterface* Character = Cast<IUSDFCharacterAttackAnimInterface>(MeshComp->GetOwner());
	if (Character)
	{
		Character->AttackFire();
	}
}
