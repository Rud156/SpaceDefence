// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceDefence/Placeables/TD_PlaceablesActors.h"
#include "SpaceDefence/Player/FPPlayer.h"
#include "SpaceDefence/Utils/Structs.h"

#include "BaseEnemy.generated.h"

UCLASS()
class SPACEDEFENCE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	FName HeadCollisionLocation = "HeadCollider";

	FTimerHandle TimerHandle;
	virtual void Attack();

protected:
	UPROPERTY(Category = EnemyHealth, VisibleAnywhere, BlueprintReadOnly)
		class UHealthAndDamageComp* HealthAndDamage;

	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly)
		class UBoxComponent* HeadColliderComponent;

	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|Enemy Settings", EditAnywhere)
		FEnemySettings Settings;

	UPROPERTY(Category = "Enemy|Enemy Settings", EditAnywhere, BlueprintReadWrite)
		bool bIsItemWithinRange = false;

	UPROPERTY(Category = "Enemy|Enemy Settings", EditAnywhere, BlueprintReadWrite)
		bool bIsPlayerWithInRange = false;
	UPROPERTY(Category = "Enemy Settings", EditAnywhere, BlueprintReadWrite)
		float DamageAmount = 30;
	UPROPERTY(Category = "Enemy Settings", EditAnywhere, BlueprintReadWrite)
		float HeadShotMultiplier = 3;
	UPROPERTY(Category = "Enemy Settings", EditAnywhere, BlueprintReadWrite)
		float AmountToGive = 20;

	UPROPERTY(Category = "Enemy|Enemy Settings", EditAnywhere, BlueprintReadWrite)
		bool bDeathBodyNeedsToBeRemoved = true;

	UPROPERTY(Category = "Enemy|Enemy Settings", EditAnywhere, BlueprintReadWrite)
		float DeadBodyTimer = 5;

	UPROPERTY(Category = "Enemy|Optimization", VisibleAnywhere, BlueprintReadWrite)
		bool bClothSimulation = false;

	UPROPERTY(Category = "Enemy|Optimization", EditAnywhere, BlueprintReadWrite)
		bool bCanAffectNavigation = false;

	UPROPERTY(Category = "Enemy|Optimization", EditAnywhere, BlueprintReadWrite)
		bool bCastShadow = false;

	UPROPERTY(Category = "Enemy|AI Settings", VisibleAnywhere, BlueprintReadWrite)
		ATD_PlaceablesActors* CurrentAttackingItem = nullptr;

	UPROPERTY(Category = "Enemy|AI Settings", VisibleAnywhere, BlueprintReadWrite)
		AFPPlayer* CurrentAttackingPlayer = nullptr;

	UPROPERTY(Category = "Enemy|AI Settings", VisibleAnywhere, BlueprintReadWrite)
		TArray<ATD_PlaceablesActors*> ListOfCurrentTargetAbleItems;

	UPROPERTY(Category = "Enemy|AI Settings", VisibleAnywhere, BlueprintReadWrite)
		TArray<AFPPlayer*> ListOfCurrentTargetAblePlayers;

	ATD_CurrencyManager* CurrencyManagerRef = nullptr;

	UFUNCTION(BlueprintCallable)
		void RemoveCurrentAttackItem() { if (CurrentAttackingItem)CurrentAttackingItem->RemoveMaxEnemiesAttackingCount(); CurrentAttackingItem = nullptr; }

#pragma endregion

	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayRandomSoundWhenMoving();


protected:
	bool bIsAlive = true;
};
