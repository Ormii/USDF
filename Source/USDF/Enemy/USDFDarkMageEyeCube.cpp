// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/USDFDarkMageEyeCube.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AUSDFDarkMageEyeCube::AUSDFDarkMageEyeCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CDO
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

	RootComponent = Mesh;
	BaseEffect->SetupAttachment(RootComponent);
	BaseEffect->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
}

// Called when the game starts or when spawned
void AUSDFDarkMageEyeCube::BeginPlay()
{
	Super::BeginPlay();
	BaseEffect->Activate(true);
	
}

// Called every frame
void AUSDFDarkMageEyeCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddWorldRotation(FRotator(1.0f, 0.4f, 0.1f));
}

