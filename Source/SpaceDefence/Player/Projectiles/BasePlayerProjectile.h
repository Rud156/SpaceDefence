// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlayerProjectile.generated.h"

UCLASS()
class SPACEDEFENCE_API ABasePlayerProjectile : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Projectile, VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* ProjectileRoot;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USphereComponent* ProjectileCollider;

protected:
	virtual void BeginPlay() override;

public:
	ABasePlayerProjectile();
	virtual void Tick(float DeltaTime) override;
};
