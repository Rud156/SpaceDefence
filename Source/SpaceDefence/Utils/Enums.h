// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementState: uint8
{
	None,
	Walk,
	Run,
	Jump,
	RunJump,
	Crouch,
	Slide
};



//PlaceAble Actor Upgrades
UENUM(BlueprintType)
enum class EPlaceAbleUpgrades : uint8
{
	None,
	Ability1,
	Ability2
};

