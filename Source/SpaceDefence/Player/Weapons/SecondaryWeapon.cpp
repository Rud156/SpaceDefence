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
		_interactionStarted = false;
		return true;
	}

	return false;
}

void ASecondaryWeapon::CancelInteraction_Implementation()
{
	_interactionStarted = false;
}

bool ASecondaryWeapon::InteractionStarted_Implementation()
{
	return _interactionStarted;
}

float ASecondaryWeapon::GetCurrentInteractionTime_Implementation()
{
	return  _currentInteractionTime;
}

float ASecondaryWeapon::GetBaseInteractionTime_Implementation()
{
	return _interactionDuration;
}

float ASecondaryWeapon::GetInteractionProgress_Implementation()
{
	return 1 - (_currentInteractionTime / _interactionDuration);
}

void ASecondaryWeapon::SetInteractionTime_Implementation(float DurationMultiplier)
{
	_interactionStarted = true;
	_interactionDuration = WeaponInteractionTime * DurationMultiplier;
	_currentInteractionTime = _interactionDuration;
}
