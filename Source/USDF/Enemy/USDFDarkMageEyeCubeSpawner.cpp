// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageEyeCubeSpawner.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/USDFAIController.h"
#include "Character/USDFCharacterNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

AUSDFDarkMageEyeCubeSpawner::AUSDFDarkMageEyeCubeSpawner() : SpawnFlag(0), bIsSpawnning(false), SpawnningTime(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	// CDO
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnEffect"));

	RootComponent = SpawnPoint;
	SpawnEffect->SetupAttachment(SpawnPoint);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SpawnEffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/NS_Portal_DarkMagic.NS_Portal_DarkMagic"));
	if (SpawnEffectRef.Object)
	{
		SpawnEffect->SetAsset(SpawnEffectRef.Object);
	}

	SpawnEffect->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
}

void AUSDFDarkMageEyeCubeSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEffect->Deactivate();
}

void AUSDFDarkMageEyeCubeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSpawnning)
	{
		if (CurrentSpawnOrder.IsEmpty())
		{
			bIsSpawnning = false;
			SpawnEffect->Deactivate();
			return;
		}

		SpawnningTime += DeltaTime;

		TQueue<FDarkMageEyeCubeSpawnOrder> TempQue;
		while (!CurrentSpawnOrder.IsEmpty())
		{
			FDarkMageEyeCubeSpawnOrder SpawnOrder;
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
			FDarkMageEyeCubeSpawnOrder SpawnOrder;
			if (!TempQue.Dequeue(SpawnOrder))
				break;

			CurrentSpawnOrder.Enqueue(SpawnOrder);
		}
	}
}

void AUSDFDarkMageEyeCubeSpawner::PrepareSpawn(int32 InSpawnFlag)
{
	SpawnFlag = InSpawnFlag;
	CurrentSpawnOrder.Empty();

	for (int32 i = 0; i < TotalSpawnOrder.Num(); ++i)
	{
		if (TotalSpawnOrder[i].SpawnFlag != InSpawnFlag)
			continue;

		CurrentSpawnOrder.Enqueue(TotalSpawnOrder[i]);
	}
}

void AUSDFDarkMageEyeCubeSpawner::BeginSpawn()
{
	bIsSpawnning = true;
	SpawnningTime = 0;
	SpawnEffect->Activate();
}

void AUSDFDarkMageEyeCubeSpawner::Spawn(const FDarkMageEyeCubeSpawnOrder& SpawnOrder)
{
	AUSDFCharacterNonPlayer* NonPlayer = GetWorld()->SpawnActorDeferred<AUSDFCharacterNonPlayer>(SpawnMonsterClass, FTransform::Identity);
	if (NonPlayer == nullptr)
		return;

	FTransform Transform = FTransform::Identity;
	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(GetActorRotation().Quaternion());
	Transform.SetScale3D(FVector(0.7f,0.7f,0.7f));
	NonPlayer->FinishSpawning(Transform);

	AUSDFAIController* PreController = Cast<AUSDFAIController>(NonPlayer->GetController());
	if (PreController == nullptr)
		return;

	AUSDFAIController* AIController = GetWorld()->SpawnActor<AUSDFAIController>(SpawnOrder.AIControllerClass);
	if (AIController == nullptr)
		return;

	PreController->StopAI();
	NonPlayer->UnPossessed();
	PreController->Destroy();
	AIController->Possess(Cast<APawn>(NonPlayer));
		
	AIController->SetCurrentAIState(EAIState::Investigating, FAISensedParam{});

	NonPlayer->GetCharacterMovement()->AddForce(GetActorForwardVector() * 600.0f);
}