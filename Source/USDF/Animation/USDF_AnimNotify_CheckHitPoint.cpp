// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDF_AnimNotify_CheckHitPoint.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"

UUSDF_AnimNotify_CheckHitPoint::UUSDF_AnimNotify_CheckHitPoint()
{

}

void UUSDF_AnimNotify_CheckHitPoint::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IUSDFCharacterPlayerAnimInterface* Player = Cast<IUSDFCharacterPlayerAnimInterface>(MeshComp->GetOwner());
	if (Player)
	{
		Player->CheckApplyDamagePoint();
	}
}
