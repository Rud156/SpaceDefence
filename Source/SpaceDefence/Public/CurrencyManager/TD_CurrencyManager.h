// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TD_CurrencyManager.generated.h"

UCLASS()
class SPACEDEFENCE_API ATD_CurrencyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATD_CurrencyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Game Play variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Currency Manager")
		int Currency = 100;



	//Game Play Functions.

	UFUNCTION(BlueprintCallable)
		bool HasCurrency(const int PlaceAbleActorCurrency);

	UFUNCTION(BlueprintCallable)
		void AddCurrency(int Amount);

	UFUNCTION(BlueprintCallable)
		void RemoveCurrency(int PlaceAbleActorCurrency);
	
	UFUNCTION(BlueprintCallable)
		void SetCurrency(int InitialCost) { Currency = InitialCost; }
	
	UFUNCTION(BlueprintCallable)
		int GetCurrency() { return Currency; }
	
};
