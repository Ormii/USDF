// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/USDFTriggerBase.h"
#include "Components/BoxComponent.h"
#include "Physics/USDFCollision.h"

// Sets default values
AUSDFTriggerBase::AUSDFTriggerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CDO
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	RootComponent = BoxTrigger;
	BoxTrigger->SetCollisionProfileName(CPROFILE_USDF_TRIGGER);
	BoxTrigger->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AUSDFTriggerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSDFTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

