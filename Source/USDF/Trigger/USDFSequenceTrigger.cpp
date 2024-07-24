// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/USDFSequenceTrigger.h"
#include "LevelSequencePlayer.h"
#include "Components/BoxComponent.h"
#include "Interface/USDFCharacterPlayerInterface.h"
#include "GameFramework/PlayerController.h"

AUSDFSequenceTrigger::AUSDFSequenceTrigger()
{

}

void AUSDFSequenceTrigger::BeginPlay()
{
	Super::BeginPlay();

    if (SequenceAsset && SequencePlayer == nullptr)
        SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SequenceAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);

    BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AUSDFSequenceTrigger::OnOverlappedBegin);


    FScriptDelegate ScriptDelegate;
    ScriptDelegate.BindUFunction(this, "OnSequenceFinished");
    SequencePlayer->OnFinished.AddUnique(ScriptDelegate);
}

void AUSDFSequenceTrigger::OnOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    IUSDFCharacterPlayerInterface* Character = Cast<IUSDFCharacterPlayerInterface>(OtherActor);
    if (Character)
    {
        //Sequence Play
        if (SequencePlayer && SequenceAsset)
        {
            APlayerController* PlayerController = Character->GetPlayerController();
            if (PlayerController)
            {
                TriggeredPlayerController = PlayerController;
                OtherActor->DisableInput(PlayerController);
                SequencePlayer->Play();
            }
        }
    }
}

void AUSDFSequenceTrigger::OnSequenceFinished()
{
    if (TriggeredPlayerController == nullptr)
        return;

    TriggeredPlayerController->GetPawn()->EnableInput(TriggeredPlayerController);
    Destroy();
}
