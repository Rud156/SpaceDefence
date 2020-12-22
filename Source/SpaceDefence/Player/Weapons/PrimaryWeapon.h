// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseWeapon.h"

#include "PrimaryWeapon.generated.h"

UCLASS()
class SPACEDEFENCE_API APrimaryWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	APrimaryWeapon();
	virtual void Tick(float DeltaTime) override;
};
