// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceDefence/Enemies/BaseEnemy.h"

#include "TD_CreepyEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SPACEDEFENCE_API ATD_CreepyEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	UPROPERTY(Category = "Creepy Settings", BlueprintReadWrite, EditAnywhere)
		ECreepyAnimState CurrentAnimationState = ECreepyAnimState::Idle;
	UPROPERTY(Category = "Creepy Settings", BlueprintReadWrite, VisibleAnywhere)
		bool bIsAlive = true;
};
