// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CurrencyManager/TD_CurrencyManager.h"
#include "GameFramework/Actor.h"
#include "SpaceDefence/Utils/Structs.h"
#include "TD_PlaceablesActors.generated.h"

UCLASS()
class SPACEDEFENCE_API ATD_PlaceablesActors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATD_PlaceablesActors();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//Game play variables.
	bool bCanPlace = false;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		FPlaceAbleData ActorData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Place Able Settings")
		USceneComponent* RootMeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Place Able Settings")
		UStaticMeshComponent* Model = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Place Able Settings")
		float MaxEnemiesWhichCanAttackAtATime = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LeftSnapPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float RightSnapPoint;
	
	//Game play functions

	UFUNCTION(BlueprintCallable)
		void ApplyDamage(float Amount);
	UFUNCTION(BlueprintCallable)
		void IncreaseMaxEnemiesAttackingCount() { CurrentEnemiesAttackingCount++; }
	UFUNCTION(BlueprintCallable)
		void RemoveMaxEnemiesAttackingCount() { if(CurrentEnemiesAttackingCount>0)CurrentEnemiesAttackingCount--; }
	UFUNCTION(BlueprintCallable)
		bool CanEnemiesAttack() const;
	UFUNCTION(BlueprintCallable)
		bool IsActive() { return bIsAlive; }
	//Non blueprint functions calls
	void SetData(FPlaceAbleData Data);

	int GetDestructionCost() const;

	void RemoveActor();
	void UpgradeActor();
private:
	bool bIsAlive = true;
	AGameModeBase* GameMode = nullptr;
	ATD_CurrencyManager* CurrencyManager = nullptr;
	float CurrentEnemiesAttackingCount = 0;
	
	void CheckIfAlive();
};
