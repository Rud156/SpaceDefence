// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryWeapon.h"
#include "../Public/Projectile/TD_BaseProjectile.h"
#include "../Common/InteractionComponent.h"

APrimaryWeapon::APrimaryWeapon() : Super()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void APrimaryWeapon::BeginPlay()
{
	Super::BeginPlay();
	_totalAmmoLeft = 300;
	_magAmmoLeft = MagSize;
}

void APrimaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrimaryWeapon::Shoot()
{
	_magAmmoLeft -= 1;
	if (_magAmmoLeft <= 0)
	{
		_reloadTime = ReloadTime;
	}
}

TSubclassOf<AActor> APrimaryWeapon::GetProjectile()
{
	return Projectile;
}

UInteractionComponent* APrimaryWeapon::GetInteractionComponent_Implementation()
{
	return InteractionComponent;
}
