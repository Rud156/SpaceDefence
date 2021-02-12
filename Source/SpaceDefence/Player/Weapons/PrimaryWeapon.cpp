// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryWeapon.h"
#include "../Public/Projectile/TD_BaseProjectile.h"
#include "../Common/InteractionComponent.h"

APrimaryWeapon::APrimaryWeapon() : Super()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void APrimaryWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APrimaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TSubclassOf<AActor> APrimaryWeapon::GetProjectile()
{
	return Projectile;
}

UInteractionComponent* APrimaryWeapon::GetInteractionComponent_Implementation()
{
	return InteractionComponent;
}