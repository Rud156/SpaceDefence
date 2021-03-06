// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthAndDamageComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitDied, AActor*, Unit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, int, NewHealth);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEDEFENCE_API UHealthAndDamageComp : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Replicated)
	int _currentHealth;

	UPROPERTY(Category = "Health", BlueprintReadOnly, EditAnywhere, Replicated)
	int MaxHealth;

	UPROPERTY(Category = "Delegates", BlueprintAssignable)
	FUnitDied OnUnitDied;

	UPROPERTY(Category = "Delegates", BlueprintAssignable)
	FHealthChanged OnHealthChanged;

#pragma endregion

	UHealthAndDamageComp(const class FObjectInitializer& PCIP);
	
	UFUNCTION(Category = "Health", BlueprintPure, BlueprintCallable)
	int GetCurrentHealth();

	void SetMaxHealth(int HealthAmount, bool ResetCurrentHealth = true);
	void AddHealth(int HealthAmount);
	void TakeDamage(int DamageAmount);

	UFUNCTION(Server, Reliable)
	void Server_TakeDamage(int DamageAmount);
	UFUNCTION(Server, Reliable)
	void Server_AddHealth(int HealthAmount);
	UFUNCTION(Server, Reliable)
	void Server_SetMaxHealth(int HealthAmount, bool ResetCurrentHealth = true);
};
