// Fill out your copyright notice in the Description page of Project Settings.


#include "MutantEnemy.h"

AMutantEnemy::AMutantEnemy() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMutantEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AMutantEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMutantEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMutantEnemy::TeleportStart()
{

}

void AMutantEnemy::TeleportEnd()
{

}