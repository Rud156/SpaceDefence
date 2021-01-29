// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "../Common/HealthAndDamageComp.h"

ABaseEnemy::ABaseEnemy()
{
	HealthAndDamage = CreateDefaultSubobject<UHealthAndDamageComp>(TEXT("HealthAndDamage"));

	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::Attack()
{

}