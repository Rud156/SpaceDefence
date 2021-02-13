// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceDefence/Enemies/BaseEnemy.h"

#include "TD_CreepyEnemy.generated.h"

/**
 *
 */
UCLASS()
class SPACEDEFENCE_API ATD_CreepyEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	ATD_CreepyEnemy();
	void BeginPlay() override;

	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly)
		class UBoxComponent* HandCollider;

	UPROPERTY(Category = "Creepy Settings", BlueprintReadWrite, EditAnywhere)
		ECreepyAnimState CurrentAnimationState = ECreepyAnimState::Idle;

	UPROPERTY(Category = "Creepy Settings", BlueprintReadWrite, VisibleAnywhere)
		bool bIsAlive = true;


	UFUNCTION()
		void OnHandColliderOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION()
		void OnCompHitHead(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void Death(AActor* Actor);

	void RemoveDeadBody() { this->Destroy(); }

private:
	FName HandBoxCollisionLocation = "HandBoxCollision";
	FTimerHandle DeathTimerHandle;
};
