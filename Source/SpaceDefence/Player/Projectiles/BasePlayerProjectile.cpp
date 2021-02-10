// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerProjectile.h"
#include "../Common/HealthAndDamageComp.h"

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

	ProjectileCollider->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayerProjectile::HandlePotionCollisionEnter);
	_destroyTime = ProjectileDestroyTime;
}

void ABasePlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_destroyTime -= DeltaTime;
	if (_destroyTime <= 0)
	{
		Destroy();
	}
}

void ABasePlayerProjectile::HandlePotionCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UActorComponent* actorComponent = OtherActor->GetComponentByClass(UHealthAndDamageComp::StaticClass());
	UHealthAndDamageComp* healthAndDamage = Cast<UHealthAndDamageComp>(actorComponent);

	if (healthAndDamage != nullptr)
	{
		healthAndDamage->TakeDamage(ProjectileDamage);
	}

	Destroy();
}