// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerProjectile.h"

ABasePlayerProjectile::ABasePlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasePlayerProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ABasePlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

