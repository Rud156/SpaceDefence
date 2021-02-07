// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerProjectile.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABasePlayerProjectile::ABasePlayerProjectile()
{
	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollider"));

	RootComponent = ProjectileRoot;
	ProjectileMesh->SetupAttachment(ProjectileRoot);
	ProjectileCollider->SetupAttachment(ProjectileRoot);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	PrimaryActorTick.bCanEverTick = true;
}

void ABasePlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}