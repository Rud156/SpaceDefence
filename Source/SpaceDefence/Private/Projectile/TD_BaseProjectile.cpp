// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/TD_BaseProjectile.h"
#include "../../Common/HealthAndDamageComp.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ATD_BaseProjectile::ATD_BaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATD_BaseProjectile::OnHit);		// set up a notification for when this component hits something blocking

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3;
}

void ATD_BaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UActorComponent* actorComponent = OtherActor->GetComponentByClass(UHealthAndDamageComp::StaticClass());
	UHealthAndDamageComp* healthAndDamage = Cast<UHealthAndDamageComp>(actorComponent);

	if (healthAndDamage != nullptr)
	{
		healthAndDamage->TakeDamage(DamageAmount);
	}

	Destroy();
}