// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFEnemyProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/USDFCollision.h"

AUSDFEnemyProjectile::AUSDFEnemyProjectile()
{
	// Mesh
	Mesh->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);
	Mesh->SetCollisionObjectType(CCHANNEL_USDF_ENEMY_PROJECTILE);
}
