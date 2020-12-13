// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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



	//Game play functions

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		UStaticMesh* Hi();
	
	UFUNCTION(BlueprintCallable)
		void Place();

	UFUNCTION(BlueprintCallable)
		void LeftRotate();

	UFUNCTION(BlueprintCallable)
		void RightRotate();

};
