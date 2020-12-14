// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class PlayerMovementState: uint8
{
	None,
	Walk,
	Run,
	Jump,
	RunJump,
	Crouch
};
