// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndDamageComp.h"
#include "./DefenceBuffBaseComp.h"

#include "Net/UnrealNetwork.h"

UHealthAndDamageComp::UHealthAndDamageComp(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	MaxHealth = 100;

	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthAndDamageComp::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UHealthAndDamageComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UHealthAndDamageComp::GetCurrentHealth()
{
	return CurrentHealth;
}

void UHealthAndDamageComp::SetMaxHealth(int healthAmount, bool resetCurrentHealth)
{
	MaxHealth = healthAmount;
	if (resetCurrentHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthAndDamageComp::AddHealth(int healthAmount)
{
	CurrentHealth += healthAmount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	OnHealthChanged.Broadcast(CurrentHealth);
}

void UHealthAndDamageComp::TakeDamage(int damageAmount)
{
	TArray<UActorComponent*> actorComponents = GetOwner()->GetComponentsByClass(UDefenceBuffBaseComp::StaticClass());
	for (int i = 0; i < actorComponents.Num(); i++)
	{
		UDefenceBuffBaseComp* damageDebuff = Cast<UDefenceBuffBaseComp>(actorComponents[i]);
		damageAmount = damageDebuff->TakeDamage(damageAmount);
	}

	const int lastHealth = CurrentHealth;
	CurrentHealth -= damageAmount;

	if (lastHealth != CurrentHealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth);
	}

	if (CurrentHealth <= 0)
	{
		OnUnitDied.Broadcast(GetOwner());
	}
}

void UHealthAndDamageComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAndDamageComp, CurrentHealth);
	DOREPLIFETIME(UHealthAndDamageComp, MaxHealth);
}
