// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseWeapon.h"

#include "SecondaryWeapon.generated.h"

UCLASS()
class SPACEDEFENCE_API ASecondaryWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASecondaryWeapon();
	virtual void Tick(float DeltaTime) override;
};
