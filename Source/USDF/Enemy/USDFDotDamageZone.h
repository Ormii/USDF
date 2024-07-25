// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFDotDamageZone.generated.h"

UCLASS()
class USDF_API AUSDFDotDamageZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFDotDamageZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Detect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> DetectZone;

	UPROPERTY(EditAnywhere, Category = DotInfo, Meta = (AllowPrivateAccess = "true"))
	float DotInterval;

	UPROPERTY(EditAnywhere, Category = DotInfo, Meta = (AllowPrivateAccess = "true"))
	float DotDamage;
};
