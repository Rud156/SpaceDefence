// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TD_SpawnPoints.h"
#include "GameFramework/Actor.h"
#include "SpaceDefence/Utils/Structs.h"


#include "TD_WaveController.generated.h"


UCLASS()
class SPACEDEFENCE_API ATD_WaveController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATD_WaveController();
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Setttings")
		TArray<FWaveSettings> ListOfSpawnPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Setttings")
		TSubclassOf<ATD_SpawnPoints> SpawnPointBP;

	UPROPERTY(BlueprintReadOnly)
		int WaveCounter =0;

	



	//functions
	UFUNCTION(CallInEditor, Category="Editor Functions")
		void SpawnANewSpawnPoint();
	

		
	
	UFUNCTION(BlueprintCallable)
		void SpawnWave();

	//This has to be a UFUNCTION for the timer delegate. 
	UFUNCTION()
		void SpawnEnemies(FWaveGroup GroupSettings, int GroupCount);

	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		FVector GetRandomLocationWithInTheRadius(float Radius, FVector SpawnLocation);



	
	
private:




	void AddToList(ATD_SpawnPoints* SpawnPoints);

	void SpawnGroups(int GroupCount);
	//variables 
	FTimerDelegate EnemiesTimerDelegate;

	FTimerHandle GroupTimerHandle;


	//SpawnGroup Controlling variables.
	
	int SpawnGroupCount;
	
	//SpawnEnemy Controlling variables.
	int SpawnEnemyCount;




};
