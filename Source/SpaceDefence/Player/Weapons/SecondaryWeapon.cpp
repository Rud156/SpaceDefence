// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondaryWeapon.h"
#include "../Public/Projectile/TD_BaseProjectile.h"
#include "../Common/InteractionComponent.h"

ASecondaryWeapon::ASecondaryWeapon() : Super()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void ASecondaryWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASecondaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TSubclassOf<AActor> ASecondaryWeapon::GetProjectile()
{
	return Projectile;
}

UInteractionComponent* ASecondaryWeapon::GetInteractionComponent_Implementation()
{
	return InteractionComponent;
}