// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

AMeleeWeapon::AMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}