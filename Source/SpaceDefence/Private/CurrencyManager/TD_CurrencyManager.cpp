// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceDefence/Public/CurrencyManager/TD_CurrencyManager.h"

// Sets default values
UTD_CurrencyManager::UTD_CurrencyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UTD_CurrencyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTD_CurrencyManager::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


bool UTD_CurrencyManager::HasCurrency(const int PlaceAbleActorCurrency)
{
	if(PlaceAbleActorCurrency>0)
	{
		
		return (Currency - PlaceAbleActorCurrency) > -1;
	}
	return false;
}

void UTD_CurrencyManager::AddCurrency(int Amount)
{
	if(Amount>0)
	{
		Currency += Amount;
	}
}

void UTD_CurrencyManager::RemoveCurrency(int PlaceAbleActorCurrency)
{
	if(HasCurrency(PlaceAbleActorCurrency))
	{
		Currency -= PlaceAbleActorCurrency;
	}
}

