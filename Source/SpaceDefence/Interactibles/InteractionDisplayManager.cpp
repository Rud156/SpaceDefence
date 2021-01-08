// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionDisplayManager.h"

AInteractionDisplayManager::AInteractionDisplayManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractionDisplayManager::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractionDisplayManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

