// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageEyeCube.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Physics/USDFCollision.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/USDFDarkMageEyeCubeSpawner.h"

// Sets default values
AUSDFDarkMageEyeCube::AUSDFDarkMageEyeCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CDO
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	BaseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseEffect"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EffectRef(TEXT("/Game/ReferenceAsset/DarkMagicFX/Niagara/Eye/NS_EyePrison.NS_EyePrison"));
	if (EffectRef.Object)
	{
		BaseEffect->SetAsset(EffectRef.Object);
	}

	RootComponent = Scene;
	Mesh->SetupAttachment(RootComponent);
	BaseEffect->SetupAttachment(Mesh);
	BaseEffect->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));

	Mesh->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);
}



// Called when the game starts or when spawned
void AUSDFDarkMageEyeCube::BeginPlay()
{
	Super::BeginPlay();
	BaseEffect->Activate(true);
	
	TArray<AActor*> Result;
	UGameplayStatics::GetAllActorsOfClass(this, AUSDFDarkMageEyeCubeSpawner::StaticClass(), Result);

	for (auto& Spawner : Result)
		Spawners.Add(Cast<AUSDFDarkMageEyeCubeSpawner>(Spawner));
}

void AUSDFDarkMageEyeCube::BeginDestroy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
	
	Super::BeginDestroy();
}

// Called every frame
void AUSDFDarkMageEyeCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddWorldRotation(FRotator(1.0f, 0.4f, 0.1f));
}

void AUSDFDarkMageEyeCube::PrepareSpawn(int32 SpawnFlag)
{
	if (GetWorld() == nullptr)
		return;

	for (int32 i = 0; i < Spawners.Num(); ++i)
		Spawners[i]->PrepareSpawn(SpawnFlag);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		Spawn();
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.0f, false);
}

void AUSDFDarkMageEyeCube::Spawn()
{
	for (int32 i = 0; i < Spawners.Num(); ++i)
		Spawners[i]->BeginSpawn();
}