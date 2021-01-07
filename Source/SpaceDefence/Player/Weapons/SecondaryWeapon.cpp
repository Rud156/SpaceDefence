// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondaryWeapon.h"
#include "../Projectiles/BasePlayerProjectile.h"

ASecondaryWeapon::ASecondaryWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASecondaryWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASecondaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TSubclassOf<AActor> ASecondaryWeapon::GetProjectile()
{
	return Projectile;
}

EInteractibleType ASecondaryWeapon::GetInteractibleType_Implementation()
{
	return EInteractibleType::Weapon;
}

bool ASecondaryWeapon::InteractUpdate_Implementation(float DeltaTime)
{
	_currentInteractionTime -= DeltaTime;
	if (_currentInteractionTime <= 0)
	{
		return true;
	}

	return false;
}

void ASecondaryWeapon::CancelInteraction_Implementation()
{

}

float ASecondaryWeapon::GetCurrentInteractionTime_Implementation()
{
	return  _currentInteractionTime;
}

float ASecondaryWeapon::GetBaseInteractionTime_Implementation()
{
	return WeaponInteractionTime;
}

void ASecondaryWeapon::SetInteractionTime_Implementation(float DurationMultiplier)
{
	_currentInteractionTime = WeaponInteractionTime * DurationMultiplier;
}
