// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/USDFGameMode.h"

AUSDFGameMode::AUSDFGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> CharacterPlayerRef(TEXT("/Game/Blueprint/BP_USDFCharacterPlayer.BP_USDFCharacterPlayer_C"));
	if (CharacterPlayerRef.Class)
	{
		DefaultPawnClass = CharacterPlayerRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Game/Blueprint/BP_USDFPlayerController.BP_USDFPlayerController_C"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
}
