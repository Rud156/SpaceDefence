// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldPingManager.generated.h"

UCLASS()
class SPACEDEFENCE_API AWorldPingManager : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AWorldPingManager();
	virtual void Tick(float DeltaTime) override;
};
