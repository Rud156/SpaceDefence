// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceDefence/Utils/Structs.h"

#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDied);

UCLASS()
class SPACEDEFENCE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	FTimerHandle TimerHandle;

protected:
	UPROPERTY(Category = EnemyHealth, VisibleAnywhere, BlueprintReadOnly)
		class UHealthAndDamageComp* HealthAndDamage;

	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy Settings", EditAnywhere)
		FEnemySettings Settings;

	UPROPERTY(Category = "Optimization", EditAnywhere)
		bool bClothSimulation = false;

	UPROPERTY(Category = "Optimization", EditAnywhere)
		bool bCanAffectNavigation = false;

	UPROPERTY(Category = "Optimization", EditAnywhere)
		bool bCastShadow = false;

#pragma endregion

	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayRandomSoundWhenMoving();
	virtual void Attack();
};
