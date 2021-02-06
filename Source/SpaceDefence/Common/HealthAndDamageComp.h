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

private:
	int _currentHealth;

protected:
	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Health", BlueprintReadOnly, EditAnywhere)
		int MaxHealth;

	UPROPERTY(Category = "Delegates", BlueprintAssignable)
		FUnitDied OnUnitDied;

	UPROPERTY(Category = "Delegates", BlueprintAssignable)
		FHealthChanged OnHealthChanged;

#pragma endregion

	UHealthAndDamageComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Category = "Health", BlueprintPure, BlueprintCallable)
		int GetCurrentHealth();

	void AddHealth(int healthAmount);
	void TakeDamage(int damageAmount);

};
