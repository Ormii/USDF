// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/USDFAISpawner.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "AI/USDFAIController.h"

// Sets default values
AUSDFAISpawner::AUSDFAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Point0 = CreateDefaultSubobject<USceneComponent>(TEXT("Point0"));
	Point1 = CreateDefaultSubobject<USceneComponent>(TEXT("Point1"));
	Point2 = CreateDefaultSubobject<USceneComponent>(TEXT("Point2"));
	Point3 = CreateDefaultSubobject<USceneComponent>(TEXT("Point3"));
	Point4 = CreateDefaultSubobject<USceneComponent>(TEXT("Point4"));

	RootComponent = Point0;

	Point1->SetupAttachment(RootComponent);
	Point2->SetupAttachment(RootComponent);
	Point3->SetupAttachment(RootComponent);
	Point4->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUSDFAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSDFAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSpawnning)
	{
		if (CurrentSpawnOrder.IsEmpty())
		{
			bIsSpawnning = false;
			return;
		}

		SpawnningTime += DeltaTime;

		TQueue<FStageSpawnOrder> TempQue;
		while (!CurrentSpawnOrder.IsEmpty())
		{
			FStageSpawnOrder SpawnOrder;
			if (!CurrentSpawnOrder.Dequeue(SpawnOrder))
				break;

			if (SpawnningTime >= SpawnOrder.DelayTime)
			{
				Spawn(SpawnOrder);
			}
			else
			{
				TempQue.Enqueue(SpawnOrder);
			}
		}

		while (!TempQue.IsEmpty())
		{
			FStageSpawnOrder SpawnOrder;
			if (!TempQue.Dequeue(SpawnOrder))
				break;

			CurrentSpawnOrder.Enqueue(SpawnOrder);
		}
	}
}

void AUSDFAISpawner::Spawn(const FStageSpawnOrder& SpawnOrder)
{
	UE_LOG(LogTemp, Display, TEXT("Spawn Call"));
	int32 SpawnCount = SpawnOrder.SpawnMonsterCount;
	USceneComponent* Points[5] = { Point0,Point1,Point2,Point3,Point4 };

	for (int i = 0; i < SpawnCount; ++i)
	{
		FVector RayStartPoint = Points[i]->GetComponentLocation();
		FVector TargetVector = FVector(0.0f, 0.0f, -1.0f);
		float RayLength = 300.0f;

		FHitResult HitResult = {};
		FCollisionQueryParams Params(SCENE_QUERY_STAT(FindLocation), false, this);
		bool bHitted = GetWorld()->LineTraceSingleByChannel(HitResult, RayStartPoint, RayStartPoint + TargetVector * RayLength, ECC_Pawn, Params);
		
		if (bHitted)
		{
			AUSDFCharacterNonPlayer* NonPlayer = GetWorld()->SpawnActor<AUSDFCharacterNonPlayer>(SpawnMonsterClass);
			AUSDFAIController* PreController = Cast<AUSDFAIController>(NonPlayer->GetController());
			AUSDFAIController* AIController = GetWorld()->SpawnActor<AUSDFAIController>(SpawnOrder.AIControllerClass);

			PreController->StopAI();
			NonPlayer->UnPossessed();
			PreController->Destroy();
			AIController->Possess(Cast<APawn>(NonPlayer));

			float HalfLength = NonPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			float Radius = NonPlayer->GetCapsuleComponent()->GetScaledCapsuleRadius();
		
			FVector SpawnPoint = HitResult.Location + FVector(0.0f, 0.0f, HalfLength + Radius);
			NonPlayer->SetActorLocation(SpawnPoint);
			NonPlayer->SetActorRotation(GetActorRotation());	
		}
	}
}

void AUSDFAISpawner::PrepareSpawn(int32 InStageNumber)
{
	UE_LOG(LogTemp, Display, TEXT("Prepare Spawn : %d"), InStageNumber);
	StageNumber = InStageNumber;
	CurrentSpawnOrder.Empty();

	for (int32 i = 0; i < TotalStageSpawnOrder.Num(); ++i)
	{
		if (TotalStageSpawnOrder[i].TargetStageNumber != StageNumber)
			continue;

		CurrentSpawnOrder.Enqueue(TotalStageSpawnOrder[i]);
	}
}

void AUSDFAISpawner::BeginSpawn()
{
	UE_LOG(LogTemp, Display, TEXT("Begin Spawn"));
	bIsSpawnning = true;
	SpawnningTime = 0;
}

void AUSDFAISpawner::SetGoal(TMap<UClass*, int>& Goal)
{
	if (Goal.Find(SpawnMonsterClass) == nullptr)
		Goal.Add(SpawnMonsterClass, 0);

	for (int32 i = 0; i < TotalStageSpawnOrder.Num(); ++i)
	{
		if (TotalStageSpawnOrder[i].TargetStageNumber != StageNumber)
			continue;

		Goal[SpawnMonsterClass]+= TotalStageSpawnOrder[i].SpawnMonsterCount;
	}
}

