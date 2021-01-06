// Fill out your copyright notice in the Description page of Project Settings.

#include "DevelopmentTools/TD_DevelopmentTools.h"
#include "WaveSystem/TD_SpawnPoints.h"


//Settings! Event tick turned off for this actor
// Sets default values
ATD_SpawnPoints::ATD_SpawnPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATD_SpawnPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATD_SpawnPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

