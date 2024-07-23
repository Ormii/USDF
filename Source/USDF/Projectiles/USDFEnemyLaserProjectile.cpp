// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/USDFEnemyLaserProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/USDFCollision.h"

AUSDFEnemyLaserProjectile::AUSDFEnemyLaserProjectile()
{
	// Mesh
	Mesh->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);
	Mesh->SetCollisionObjectType(CCHANNEL_USDF_NON_PLAYER_CHARACTER);

	BoxCollision->SetCollisionProfileName(CPROFILE_USDF_NONPLAYER_CAPSULE);
	BoxCollision->SetCollisionObjectType(CCHANNEL_USDF_NON_PLAYER_CHARACTER);

	Mesh->SetCollisionResponseToChannel(CCHANNEL_USDF_PLAYER_CHARACTER, ECollisionResponse::ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(CCHANNEL_USDF_PLAYER_CHARACTER, ECollisionResponse::ECR_Overlap);
}
