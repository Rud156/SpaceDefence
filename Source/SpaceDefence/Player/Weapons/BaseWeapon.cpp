// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

ABaseWeapon::ABaseWeapon()
{
	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	StaticWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaitcWeaponMesh"));
	SkeletalWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalWeaponMesh"));
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));

	RootComponent = WeaponRoot;
	StaticWeaponMesh->SetupAttachment(WeaponRoot);
	SkeletalWeaponMesh->SetupAttachment(WeaponRoot);
	WeaponCollider->SetupAttachment(WeaponRoot);

	PrimaryActorTick.bCanEverTick = true;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseWeapon::ShootTick(float DeltaTime)
{
	float currentTime = GetWorld()->GetTimeSeconds();
	float difference = currentTime - _lastShotTime;
	if (difference > FireRate)
	{
		_lastShotTime = currentTime;
		return true;
	}
	else
	{
		return false;
	}
}

void ABaseWeapon::Shoot()
{
}

TSubclassOf<AActor> ABaseWeapon::GetProjectile()
{
	return nullptr;
}

void ABaseWeapon::HideWeapon()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ABaseWeapon::ShowWeapon()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}