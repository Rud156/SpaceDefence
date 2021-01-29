// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "./BaseEnemy.h"

#include "MutantEnemy.generated.h"

UCLASS()
class SPACEDEFENCE_API AMutantEnemy : public ABaseEnemy
{
	GENERATED_BODY()

private:
	bool _isAttacking;
	float _currentAttackDuration;

protected:
	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		int TeleportCountMin;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		int TeleportCountMax;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		float TempAttackDuration;

#pragma endregion 

	AMutantEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TeleportStart();
	void TeleportEnd();

	UFUNCTION(Category = "Enemy|Attack", BlueprintCallable, BlueprintPure)
		bool GetIsAttacking();
	virtual void Attack() override;
};
