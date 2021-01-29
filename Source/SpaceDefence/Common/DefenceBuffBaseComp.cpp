// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenceBuffBaseComp.h"

UDefenceBuffBaseComp::UDefenceBuffBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDefenceBuffBaseComp::BeginPlay()
{
	Super::BeginPlay();
}

void UDefenceBuffBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UDefenceBuffBaseComp::TakeDamage(int damageAmount)
{
	return damageAmount;
}