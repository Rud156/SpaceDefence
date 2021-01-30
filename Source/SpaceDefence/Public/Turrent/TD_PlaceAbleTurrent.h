// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceDefence/Placeables/TD_PlaceablesActors.h"

#include "TD_PlaceAbleTurrent.generated.h"

UCLASS()
class SPACEDEFENCE_API ATD_PlaceAbleTurrent : public ATD_PlaceablesActors
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATD_PlaceAbleTurrent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret Settings")
		float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Settings")
		float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Settings")
		bool bFireAtClosestTarget = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Settings")
		ABaseEnemy* ClosestTarget = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Settings")
		TArray<ABaseEnemy*> ListOfTargetsInRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings")
		float ClosestDistance = 999999.0f;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void FindClosestTarget();

};
