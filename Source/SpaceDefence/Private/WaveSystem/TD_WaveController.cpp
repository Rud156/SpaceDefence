// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSystem/TD_WaveController.h"
#include "DevelopmentTools/TD_DevelopmentTools.h"
#include "../../Enemies/BaseEnemy.h"


// Sets default values
ATD_WaveController::ATD_WaveController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

// Called when the game starts or when spawned
void ATD_WaveController::BeginPlay()
{
	Super::BeginPlay();

	
}

void ATD_WaveController::SpawnANewSpawnPoint()
{
	if(GetWorld() && SpawnPointBP)
	{
		FVector Loc;
		FRotator Rot;
		auto temp = Cast<ATD_SpawnPoints>(GetWorld()->SpawnActor(SpawnPointBP,&Loc,&Rot));
		temp->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		AddToList(temp);
	}
}

void ATD_WaveController::AddToList(ATD_SpawnPoints* SpawnPoints)
{
	if(SpawnPoints)
	{
	
		ListOfSpawnPoints.Add(FWaveSettings( SpawnPoints));
	}
}

void ATD_WaveController::SpawnWave()
{

	if(WaveCounter < ListOfSpawnPoints.Num())
	{
		
	auto temp = ListOfSpawnPoints[WaveCounter];
	PrintToScreen_1("Current Wave %d", WaveCounter);
	 SpawnGroups(temp.GroupSettings.Num());
	 OnWaveStart.Broadcast();
	}
}



void ATD_WaveController::SpawnGroups(int GroupCount)
{
	if(SpawnGroupCount<GroupCount)
	{
		FWaveGroup Group = ListOfSpawnPoints[WaveCounter].GroupSettings[SpawnGroupCount];

		if(!Group.OptionalSpawnLocation)
		{
			Group.OptionalSpawnLocation = ListOfSpawnPoints[WaveCounter].SpawnPoints;
		}
		EnemiesTimerDelegate.BindUFunction(this, FName("SpawnEnemies"), Group, ListOfSpawnPoints[WaveCounter].GroupSettings.Num());
		GetWorld()->GetTimerManager().SetTimer(GroupTimerHandle, EnemiesTimerDelegate, Group.EnemyInterval, true,0.0f);
		SpawnGroupCount++;
		
	}
	else
	{
		SpawnGroupCount = 0;
		WaveCounter++;
		OnWaveEndEvent.Broadcast();
		PrintToScreen("Wave spawning completed");
		GetWorld()->GetTimerManager().ClearTimer(GroupTimerHandle);
	}
}

void ATD_WaveController::SpawnEnemies(const FWaveGroup& GroupSettings, int GroupCount)
{
	if (SpawnEnemyCount < GroupSettings.EnemyCount)
	{

		
		FVector Location = GetRandomLocationWithInTheRadius(GroupSettings.SpawnRadius,GroupSettings.OptionalSpawnLocation->GetActorLocation());
		FRotator Rot (0.0f,0.0f,0.0f);
		if(GroupSettings.EnemyType!=nullptr)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			auto TempEnemy = GetWorld()->SpawnActor(GroupSettings.EnemyType.Get(), &Location, &Rot,SpawnParameters);
			
		}
		SpawnEnemyCount++;
	}
	else
	{
		SpawnEnemyCount = 0;
		SpawnGroups(GroupCount);
	}

	//


	//auto Enemy = GetWorld()->SpawnActor(L)
}



// Called every frame
void ATD_WaveController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

