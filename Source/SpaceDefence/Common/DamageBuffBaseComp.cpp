// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBuffBaseComp.h"

UDamageBuffBaseComp::UDamageBuffBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDamageBuffBaseComp::BeginPlay()
{
	Super::BeginPlay();
}

void UDamageBuffBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UDamageBuffBaseComp::ApplyDamage(int damageAmount, ABaseEnemy* enemyBase)
{
	return damageAmount;
}