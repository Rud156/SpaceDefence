// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "./BaseEnemy.h"
#include "../Utils/Enums.h"

#include "MutantEnemy.generated.h"

UCLASS()
class SPACEDEFENCE_API AMutantEnemy : public ABaseEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class UBoxComponent* HandCollider;

	float _currentTime;

	bool _isAttacking;
	int _currentAttackCount;

	EMutantTeleportState _teleportState;
	void SetMutantTeleportState(EMutantTeleportState teleportState);

protected:
	UFUNCTION()
		void HandleHandCollisionEnter(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|Teleport", EditAnywhere)
		int TeleportCountMin;

	UPROPERTY(Category = "Enemy|Teleport", EditAnywhere)
		int TeleportCountMax;

	UPROPERTY(Category = "Enemy|Teleport", EditAnywhere)
		float TeleportStartTime;

	UPROPERTY(Category = "Enemy|Teleport", EditAnywhere)
		float TeleportEndTime;

	UPROPERTY(Category = "Enemy|Teleport", EditAnywhere)
		float TeleportMoveDistance;

	UPROPERTY(Category = "Enemy|Attack", EditAnywhere)
		float MinAttackCount;

	UPROPERTY(Category = "Enemy|Attack", EditAnywhere)
		float MaxAttackCount;

	UPROPERTY(Category = "Enemy|Attack", EditAnywhere)
		int DamageAmount;

#pragma endregion 

	AMutantEnemy();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Enemy|Teleport", BlueprintCallable, BlueprintPure)
		EMutantTeleportState GetTeleportState();
	UFUNCTION(Category = "Enemy|Teleport", BlueprintImplementableEvent)
		void OnTeleportStart();
	UFUNCTION(Category = "Enemy|Teleport", BlueprintImplementableEvent)
		void OnTeleportEnd();
	UFUNCTION(Category = "Enemy|Teleport", BlueprintCallable, BlueprintPure)
		float GetTeleportMoveDistance();
	int TeleportStart();
	int TeleportEnd();

	UFUNCTION(Category = "Enemy|Attack", BlueprintCallable)
		void AttackAnimationComplete(FString attackName);
	UFUNCTION(Category = "Enemy|Attack", BlueprintCallable, BlueprintPure)
		bool GetIsAttacking();
	UFUNCTION(Category = "Enemy|Attack", BlueprintCallable, BlueprintPure)
		int GetAttackIndex();
	virtual void Attack() override;
};
