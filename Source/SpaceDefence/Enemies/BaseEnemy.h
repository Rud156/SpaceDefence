// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../Utils/ActorUtils/IntfHealthDmg.h"

#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDied);

UCLASS()
class SPACEDEFENCE_API ABaseEnemy : public ACharacter, public IIntfHealthDmg
{
	GENERATED_BODY()

protected:
	float _currentHealth;

	virtual void BeginPlay() override;

	void NotifyEnemyDied();

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|Data", EditAnywhere)
		float MaxHealth;

	UPROPERTY(Category = "Enemy|Delegates", EditAnywhere)
		FEnemyDied OnEnemyDied;

#pragma endregion

	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetCurrentHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual void SetCurrentHealth_Implementation(float HealthAmount) override;
	virtual void TakeDamage_Implementation(float DamageAmount) override;

};
