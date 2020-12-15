// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TD_PlaceablesActors.h"
#include "GameFramework/Actor.h"
#include "TD_PlaceableSpawnerActor.generated.h"

UCLASS()
class SPACEDEFENCE_API ATD_PlaceableSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATD_PlaceableSpawnerActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//Game play Variables.
	UPROPERTY(EditAnywhere,Category="Spawn Settings",BlueprintReadWrite)
	TArray<TSubclassOf<ATD_PlaceablesActors>> ListOfPlaceables;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
		TMap<TSubclassOf<ATD_PlaceablesActors>,UStaticMesh* > TestListOfPlaceables;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		ATD_PlaceablesActors* Ghost=nullptr;
	
	

	//game play functions
	UFUNCTION(BlueprintCallable)
		void SpawnPlaceAbleGhost(FVector Location, int PlaceAbleID);

	UFUNCTION(BlueprintCallable)
		UStaticMesh* GetStaticMesh(int PlaceAbleID);

};
