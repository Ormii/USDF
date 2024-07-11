// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDF_AnimNotifyState_Trail.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"

void UUSDF_AnimNotifyState_Trail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	IUSDFCharacterPlayerAnimInterface* Player = Cast<IUSDFCharacterPlayerAnimInterface>(MeshComp->GetOwner());
	if (Player)
	{
		PSTemplate = Player->GetTrailParticleSystem();
		FirstSocketName = Player->GetTrailStartSocketName();
		SecondSocketName = Player->GetTrailEndSocketName();
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UUSDF_AnimNotifyState_Trail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	IUSDFCharacterPlayerAnimInterface* Player = Cast<IUSDFCharacterPlayerAnimInterface>(MeshComp->GetOwner());
	if (Player)
	{
		PSTemplate = Player->GetTrailParticleSystem();
		FirstSocketName = Player->GetTrailStartSocketName();
		SecondSocketName = Player->GetTrailEndSocketName();
	}

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
