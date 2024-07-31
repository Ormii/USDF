// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/USDFDotDamageZone.h"
#include "USDFDarkMageDotDamageZone.generated.h"

/**
 * 
 */
UCLASS()
class USDF_API AUSDFDarkMageDotDamageZone : public AUSDFDotDamageZone
{
	GENERATED_BODY()
	
public:
	AUSDFDarkMageDotDamageZone();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void BeginActivate();

public:
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovementComponent()const { return ProjectileMovement; }

protected:
	UPROPERTY(VisibleAnywhere, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = Tag, Meta = (AllowPrivateAccess = "true"))
	int32 TagID;

	UPROPERTY(EditAnywhere, Category = Activate, Meta = (AllowPrivateAccess = "true"))
	float ActivateBeginTime;

	UPROPERTY(EditAnywhere, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	float CollisionDamage;

	UPROPERTY(VisibleAnywhere, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	TSet<TObjectPtr<AActor>> HitCharacters;

	float CurrentTime;
	bool bActivate;


protected:
	UFUNCTION()
	void OnComponentHitFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentOverlapBeginFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
