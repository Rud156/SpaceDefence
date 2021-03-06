// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndDamageComp.h"
#include "./DefenceBuffBaseComp.h"

#include "Net/UnrealNetwork.h"

UHealthAndDamageComp::UHealthAndDamageComp(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	MaxHealth = 100;

	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthAndDamageComp::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		_currentHealth = MaxHealth;
	}
}

int UHealthAndDamageComp::GetCurrentHealth()
{
	return _currentHealth;
}

void UHealthAndDamageComp::SetMaxHealth(int HealthAmount, bool ResetCurrentHealth)
{
	Server_SetMaxHealth(HealthAmount, ResetCurrentHealth);
}

void UHealthAndDamageComp::Server_SetMaxHealth_Implementation(int HealthAmount, bool ResetCurrentHealth)
{
	MaxHealth = HealthAmount;
	if (ResetCurrentHealth)
	{
		_currentHealth = MaxHealth;
	}
}

void UHealthAndDamageComp::AddHealth(int HealthAmount)
{
	Server_AddHealth(HealthAmount);
}

void UHealthAndDamageComp::Server_AddHealth_Implementation(int HealthAmount)
{
	_currentHealth += HealthAmount;
	if (_currentHealth > MaxHealth)
	{
		_currentHealth = MaxHealth;
	}

	OnHealthChanged.Broadcast(_currentHealth);
}

void UHealthAndDamageComp::TakeDamage(int DamageAmount)
{
	Server_TakeDamage(DamageAmount);
}

void UHealthAndDamageComp::Server_TakeDamage_Implementation(int DamageAmount)
{
	TArray<UActorComponent*> actorComponents = GetOwner()->GetComponentsByClass(UDefenceBuffBaseComp::StaticClass());
	for (int i = 0; i < actorComponents.Num(); i++)
	{
		UDefenceBuffBaseComp* damageDebuff = Cast<UDefenceBuffBaseComp>(actorComponents[i]);
		DamageAmount = damageDebuff->TakeDamage(DamageAmount);
	}

	const int lastHealth = _currentHealth;
	_currentHealth -= DamageAmount;

	if (lastHealth != _currentHealth)
	{
		OnHealthChanged.Broadcast(_currentHealth);
	}

	if (_currentHealth <= 0)
	{
		OnUnitDied.Broadcast(GetOwner());
	}
}

void UHealthAndDamageComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAndDamageComp, _currentHealth);
	DOREPLIFETIME(UHealthAndDamageComp, MaxHealth);
}
