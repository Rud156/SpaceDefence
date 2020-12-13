// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlaceableSpawnerActor.h"

// Sets default values
ATD_PlaceableSpawnerActor::ATD_PlaceableSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATD_PlaceableSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATD_PlaceableSpawnerActor::SpawnPlaceAbleGhost(FVector Location, int PlaceAbleID)
{

	if(GetWorld())
	{

		FRotator Rot;
	
		Ghost = Cast<ATD_PlaceablesActors>( GetWorld()->SpawnActor(ListOfPlaceables[0].Get(),&Location,&Rot));

		//Ghost->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
		
	}
	
}

UStaticMesh* ATD_PlaceableSpawnerActor::GetStaticMesh(int PlaceAbleID)
{

	//auto temp = Cast<ATD_PlaceablesActors>(ListOfPlaceables[PlaceAbleID])->Hi();
	
	return nullptr;
}


// Called every frame
void ATD_PlaceableSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

