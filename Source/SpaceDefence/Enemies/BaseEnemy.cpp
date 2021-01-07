// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
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

void ABaseEnemy::NotifyEnemyDied()
{
	OnEnemyDied.Broadcast();
}

float ABaseEnemy::GetCurrentHealth_Implementation()
{
	return  _currentHealth;
}

float ABaseEnemy::GetMaxHealth_Implementation()
{
	return  MaxHealth;
}

void ABaseEnemy::SetCurrentHealth_Implementation(float HealthAmount)
{
	_currentHealth = HealthAmount;
	if (_currentHealth > MaxHealth)
	{
		_currentHealth = MaxHealth;
	}
}

void ABaseEnemy::TakeDamage_Implementation(float DamageAmount)
{
	_currentHealth -= DamageAmount;
	if (_currentHealth <= 0)
	{
		NotifyEnemyDied();
	}
}
