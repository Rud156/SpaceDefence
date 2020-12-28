// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TD_PlaceablesActors.h"
#include "GameFramework/Actor.h"
#include "SpaceDefence/Utils/Structs.h"

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

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
		TArray<FPlaceAbleData> PlaceAbleData;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
		UMaterial* GreenColor;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
		UMaterial* RedColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ATD_PlaceablesActors* ActorRef=nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USceneComponent* RootMeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Ghost = nullptr;


	//bool

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsColliding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsOnGround = false;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsRotated = false;


	//float 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinGroundCheckDistance = 10;


	
	

	//game play functions
	UFUNCTION(BlueprintCallable)
		void SpawnPlaceAbleGhost(FVector Location, int PlaceAbleID);
	UFUNCTION(BlueprintCallable)
		void ChangeGhost(int PlaceAbleID);
	
	UFUNCTION(BlueprintCallable)
		void SpawnActorFromGhost( int PlaceAbleID);

	UFUNCTION(BlueprintCallable)
		void RemoveGhost();
	
	UFUNCTION(BlueprintCallable)
		void SetGhostPosition(FVector Location, FRotator Rot, FVector Offset);

	UFUNCTION(BlueprintCallable)
		void ChangeColor();

	UFUNCTION(BlueprintCallable)
		UStaticMesh* GetStaticMeshFromList(int PlaceAbleID);

	UFUNCTION(BlueprintCallable)
		bool CanPlace();
	
	UFUNCTION(BlueprintCallable)
		void RotateLeft(FRotator Rot);

	UFUNCTION(BlueprintCallable)
		void RotateRight(FRotator Rot);
	
	UFUNCTION(BlueprintCallable)
		bool CheckIsOnGround();

	UFUNCTION(BlueprintCallable)
		bool CanSpawnGhost(int Cost);
};
