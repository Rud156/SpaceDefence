// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlaceablesActors.h"

// Sets default values
ATD_PlaceablesActors::ATD_PlaceablesActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATD_PlaceablesActors::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATD_PlaceablesActors::Place()
{
}

void ATD_PlaceablesActors::LeftRotate()
{
}

void ATD_PlaceablesActors::RightRotate()
{
}

// Called every frame
void ATD_PlaceablesActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

