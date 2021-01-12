// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldPingManager.h"

AWorldPingManager::AWorldPingManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldPingManager::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldPingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

