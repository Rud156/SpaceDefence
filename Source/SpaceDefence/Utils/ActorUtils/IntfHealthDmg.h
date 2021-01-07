// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IntfHealthDmg.generated.h"

UINTERFACE(BlueprintType)
class SPACEDEFENCE_API UIntfHealthDmg : public UInterface
{
	GENERATED_BODY()
};


class SPACEDEFENCE_API IIntfHealthDmg
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintNativeEvent)
		float GetCurrentHealth();

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintNativeEvent)
		float GetMaxHealth();

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintNativeEvent)
		void SetCurrentHealth(float HealthAmount);

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintNativeEvent)
		void TakeDamage(float DamageAmount);
};