// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/USDFAnimNotify_UnequipWeapon.h"
#include "Interface/USDFCharacterPlayerAnimInterface.h"

void UUSDFAnimNotify_UnequipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IUSDFCharacterPlayerAnimInterface* PlayerAnimInterface = Cast<IUSDFCharacterPlayerAnimInterface>(MeshComp->GetOwner());
	if (PlayerAnimInterface)
	{
		PlayerAnimInterface->UnEquipWeapon();
	}
}
