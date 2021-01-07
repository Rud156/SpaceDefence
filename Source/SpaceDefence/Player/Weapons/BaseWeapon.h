// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../../Utils/Enums.h"

#include "BaseWeapon.generated.h"

UCLASS()
class SPACEDEFENCE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* WeaponRoot;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class UStaticMeshComponent* StaticWeaponMesh;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USkeletalMeshComponent* SkeletalWeaponMesh;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class UBoxComponent* WeaponCollider;

	virtual void BeginPlay() override;

	float _lastShotTime;

public:
#pragma region Properties

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		EPlayerWeapon WeaponType;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		float FireRate;
	
#pragma endregion

	ABaseWeapon();
	virtual void Tick(float DeltaTime) override;
	
	virtual void Shoot();

	virtual void HideWeapon();
	virtual void ShowWeapon();

	virtual bool ShootTick(float DeltaTime);
	virtual TSubclassOf<class AActor> GetProjectile();
};
