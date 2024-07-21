// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFBossDarkMageSpawner.h"
#include "Character/USDFCharacterBossDarkMage.h"
#include "AI/USDFBossDarkMageAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/USDFGameModeInterface.h"

AUSDFBossDarkMageSpawner::AUSDFBossDarkMageSpawner()
{
	static ConstructorHelpers::FClassFinder<AUSDFCharacterBossDarkMage> SpawnMonsterClassRef(TEXT("/Script/USDF.USDFCharacterBossDarkMage"));
	if (SpawnMonsterClassRef.Class)
	{
		SpawnMonsterClass = SpawnMonsterClassRef.Class;
	}
}

void AUSDFBossDarkMageSpawner::Spawn(const FStageSpawnOrder& SpawnOrder)
{
	UE_LOG(LogTemp, Display, TEXT("Spawn Call"));
	int32 SpawnCount = SpawnOrder.SpawnMonsterCount;
	USceneComponent* Point = Point0;

	FVector RayStartPoint = Point->GetComponentLocation();
	FVector TargetVector = FVector(0.0f, 0.0f, -1.0f);
	float RayLength = 300.0f;

	FHitResult HitResult = {};
	FCollisionQueryParams Params(SCENE_QUERY_STAT(FindLocation), false, this);
	bool bHitted = GetWorld()->LineTraceSingleByChannel(HitResult, RayStartPoint, RayStartPoint + TargetVector * RayLength, ECC_Visibility, Params);

	if (bHitted)
	{
		AUSDFCharacterBossDarkMage* BossDarkMage = GetWorld()->SpawnActor<AUSDFCharacterBossDarkMage>(SpawnMonsterClass);
		AUSDFAIController* PreController = Cast<AUSDFAIController>(BossDarkMage->GetController());
		AUSDFBossDarkMageAIController* AIController = GetWorld()->SpawnActor<AUSDFBossDarkMageAIController>(SpawnOrder.AIControllerClass);

		PreController->StopAI();
		BossDarkMage->UnPossessed();
		PreController->Destroy();
		AIController->Possess(Cast<APawn>(BossDarkMage));

		float HalfLength = BossDarkMage->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		float Radius = BossDarkMage->GetCapsuleComponent()->GetScaledCapsuleRadius();

		FVector SpawnPoint = HitResult.Location + FVector(0.0f, 0.0f, HalfLength + Radius);
		BossDarkMage->SetActorLocation(SpawnPoint);
		BossDarkMage->SetActorRotation(GetActorRotation());

		IUSDFGameModeInterface* GameModeInterface = Cast<IUSDFGameModeInterface>(GetWorld()->GetAuthGameMode());
		if (GameModeInterface)
		{
			GameModeInterface->OnBossEntry(BossDarkMage);
		}
	}
}
