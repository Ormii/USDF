// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USDFProjectileBase.generated.h"

UCLASS()
class USDF_API AUSDFProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSDFProjectileBase();

protected:
	virtual void PostInitializeComponents()override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> BaseEffect;

	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class UNiagaraSystem> HitEffect;

	// Damage Section
protected:
	UPROPERTY(EditAnywhere, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

public:
	FORCEINLINE void SetAttackDamage(float NewAttackDamage) { AttackDamage = NewAttackDamage; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }

	class UStaticMeshComponent* GetMesh() const { return Mesh; }
	class UProjectileMovementComponent* GetProjectileMovementComp() const {return ProjectileMovement;}

protected:
	UFUNCTION()
	void OnComponentHitFunc(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	FTimerHandle TimerHandle;
};
