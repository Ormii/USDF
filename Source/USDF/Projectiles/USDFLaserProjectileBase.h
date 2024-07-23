// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFLaserProjectileBase.generated.h"

UCLASS()
class USDF_API AUSDFLaserProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFLaserProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> BaseEffect;

	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCollision;

	// Damage Section
protected:
	UPROPERTY(EditAnywhere, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	TSet<TObjectPtr<AActor>> HitCharacters;

public:
	FORCEINLINE void SetAttackDamage(float NewAttackDamage) { AttackDamage = NewAttackDamage; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }

protected:

	UFUNCTION()
	void OnOverlapBeginFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
