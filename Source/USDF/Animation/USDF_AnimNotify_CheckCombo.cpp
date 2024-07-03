// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDF_AnimNotify_CheckCombo.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"

UUSDF_AnimNotify_CheckCombo::UUSDF_AnimNotify_CheckCombo()
{

}

void UUSDF_AnimNotify_CheckCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(MeshComp->GetOwner());
	if (PlayerAnimInterface)
	{
		PlayerAnimInterface->CheckCombo();
	}
}
