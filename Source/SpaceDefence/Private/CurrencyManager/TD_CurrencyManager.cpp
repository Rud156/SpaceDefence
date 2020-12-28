// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceDefence/Public/CurrencyManager/TD_CurrencyManager.h"

// Sets default values
ATD_CurrencyManager::ATD_CurrencyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATD_CurrencyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATD_CurrencyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATD_CurrencyManager::HasCurrency(const int PlaceAbleActorCurrency)
{
	if(PlaceAbleActorCurrency>0)
	{
		
		return (Currency - PlaceAbleActorCurrency) > 0;
	}
	return false;
}

void ATD_CurrencyManager::AddCurrency(int Amount)
{
	if(Amount>0)
	{
		Currency += Amount;
	}
}

void ATD_CurrencyManager::RemoveCurrency(int PlaceAbleActorCurrency)
{
	if(HasCurrency(PlaceAbleActorCurrency))
	{
		Currency -= PlaceAbleActorCurrency;
	}
}

