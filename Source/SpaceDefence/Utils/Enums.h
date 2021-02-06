// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

#pragma region Player

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	None,
	Walk,
	Run,
	Jump,
	RunJump,
	Crouch,
	Slide
};

UENUM(BlueprintType)
enum class EPlayerWeapon : uint8
{
	Melee,
	Primary,
	Secondary
};

#pragma endregion

#pragma region Interactibles

UENUM(BlueprintType)
enum class EInteractibleType : uint8
{
	Weapon
};

#pragma endregion

#pragma region Upgrades

//PlaceAble Actor Upgrades
UENUM(BlueprintType)
enum class EPlaceAbleUpgrades : uint8
{
	None,
	Ability1,
	Ability2
};

#pragma endregion 

#pragma region Enemies

// Mutant Enemy
UENUM(BlueprintType)
enum class EMutantState : uint8
{
	IdleWander,
	Attack,
	Wait
};

// Mutant Enemy
UENUM(BlueprintType)
enum class EMutantTeleportState : uint8
{
	None,
	TeleportStart,
	TeleportEnd,
};

//Creepy
UENUM(BlueprintType)
enum class ECreepyAnimState : uint8
{
	None,
	Attack,
	AttackItem,
	AttackPlayer,
	Dead,
	Movement,
	Run,
	Walk,
	Idle
};
#pragma endregion