// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryWeapon.h"
#include "../Projectiles/BasePlayerProjectile.h"

APrimaryWeapon::APrimaryWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APrimaryWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APrimaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TSubclassOf<AActor> APrimaryWeapon::GetProjectile()
{
	return Projectile;
}

EInteractibleType APrimaryWeapon::GetInteractibleType_Implementation()
{
	return EInteractibleType::Weapon;
}

bool APrimaryWeapon::InteractUpdate_Implementation(float DeltaTime)
{
	_currentInteractionTime -= DeltaTime;
	if (_currentInteractionTime <= 0)
	{
		_interactionStarted = false;
		return true;
	}

	return false;
}

void APrimaryWeapon::CancelInteraction_Implementation()
{
	_interactionStarted = false;
}

bool APrimaryWeapon::InteractionStarted_Implementation()
{
	return _interactionStarted;
}

float APrimaryWeapon::GetCurrentInteractionTime_Implementation()
{
	return _currentInteractionTime;
}

float APrimaryWeapon::GetBaseInteractionTime_Implementation()
{
	return _interactionDuration;
}

float APrimaryWeapon::GetInteractionProgress_Implementation()
{
	return 1 - (_currentInteractionTime / _interactionDuration);
}

void APrimaryWeapon::SetInteractionTime_Implementation(float DurationMultiplier)
{
	_interactionStarted = true;
	_interactionDuration = WeaponInteractionTime * DurationMultiplier;
	_currentInteractionTime = _interactionDuration;
}