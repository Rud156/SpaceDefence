// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDied);

UCLASS()
class SPACEDEFENCE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = EnemyHealth, VisibleAnywhere, BlueprintReadOnly)
		class UHealthAndDamageComp* HealthAndDamage;

	virtual void BeginPlay() override;

public:
	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual int Attack();
};
