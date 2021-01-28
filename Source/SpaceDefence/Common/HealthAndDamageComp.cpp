// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndDamageComp.h"
#include "./DefenceBuffBaseComp.h"

UHealthAndDamageComp::UHealthAndDamageComp()
{
	MaxHealth = 100;

	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthAndDamageComp::BeginPlay()
{
	Super::BeginPlay();

	_currentHealth = MaxHealth;
}

void UHealthAndDamageComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UHealthAndDamageComp::GetCurrentHealth()
{
	return _currentHealth;
}

void UHealthAndDamageComp::AddHealth(int healthAmount)
{
	_currentHealth += healthAmount;
	if (_currentHealth > MaxHealth)
	{
		_currentHealth = MaxHealth;
	}

	OnHealthChanged.Broadcast(_currentHealth);
}

void UHealthAndDamageComp::TakeDamage(int damageAmount)
{
	TArray<UActorComponent*> actorComponents = GetOwner()->GetComponentsByClass(UDefenceBuffBaseComp::StaticClass());
	for (int i = 0; i < actorComponents.Num(); i++)
	{
		UDefenceBuffBaseComp* damageDebuff = Cast<UDefenceBuffBaseComp>(actorComponents[i]);
		damageAmount = damageDebuff->TakeDamage(damageAmount);
	}

	_currentHealth -= damageAmount;
	OnHealthChanged.Broadcast(_currentHealth);

	if (_currentHealth <= 0)
	{
		OnUnitDied.Broadcast(GetOwner());
	}
}