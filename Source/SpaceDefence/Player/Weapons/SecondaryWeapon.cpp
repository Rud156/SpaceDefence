// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondaryWeapon.h"

ASecondaryWeapon::ASecondaryWeapon()
{
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

