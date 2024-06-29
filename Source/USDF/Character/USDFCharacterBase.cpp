// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USDFCharacterBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AUSDFCharacterBase::AUSDFCharacterBase()
{
	RootComponent = GetCapsuleComponent();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ReferenceAsset/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/ReferenceAsset/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

}

// Called when the game starts or when spawned
void AUSDFCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called to bind functionality to input
void AUSDFCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

