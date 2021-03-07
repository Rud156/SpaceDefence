// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TD_CurrencyManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEDEFENCE_API UTD_CurrencyManager : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UTD_CurrencyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


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
