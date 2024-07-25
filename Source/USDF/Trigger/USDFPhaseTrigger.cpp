// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/USDFPhaseTrigger.h"
#include "Components/BoxComponent.h"
#include "Interface/USDFGameStageInterface.h"
#include "Interface/USDFCharacterPlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Game/USDFGameStage.h"

AUSDFPhaseTrigger::AUSDFPhaseTrigger()
{
	GameStagePhase = EGameStagePhase::EGameStagePhase_Intro;
}

void AUSDFPhaseTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AUSDFPhaseTrigger::OnOverlappedBegin);
}

void AUSDFPhaseTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUSDFPhaseTrigger::OnOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* GameStage = UGameplayStatics::GetActorOfClass(this, AUSDFGameStage::StaticClass());
	if (GameStage)
	{
		IUSDFGameStageInterface* GameStageInterface = Cast<IUSDFGameStageInterface>(GameStage);
		if (GameStageInterface)
		{
			GameStageInterface->SetGameStage(GameStagePhase);
			Destroy();
		}
	}
}
