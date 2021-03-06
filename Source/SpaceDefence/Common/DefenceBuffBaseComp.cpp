// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenceBuffBaseComp.h"

UDefenceBuffBaseComp::UDefenceBuffBaseComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDefenceBuffBaseComp::BeginPlay()
{
	Super::BeginPlay();
}

int UDefenceBuffBaseComp::TakeDamage(int damageAmount)
{
	return damageAmount;
}