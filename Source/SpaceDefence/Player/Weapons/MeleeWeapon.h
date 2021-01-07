// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseWeapon.h"

#include "MeleeWeapon.generated.h"

UCLASS()
class SPACEDEFENCE_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AMeleeWeapon();
	virtual void Tick(float DeltaTime) override;
};
