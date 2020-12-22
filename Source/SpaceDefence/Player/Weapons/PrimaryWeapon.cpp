// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryWeapon.h"

APrimaryWeapon::APrimaryWeapon()
{
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

