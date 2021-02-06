// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlaceablesActors.h"


#include "TD_GameModeFPS.h"
#include "CurrencyManager/TD_CurrencyManager.h"

#include "Kismet/GameplayStatics.h"

#include "SpaceDefence/Public/DevelopmentTools/TD_DevelopmentTools.h"


#define ZER0 0.0

//#include "SpaceDefence/Utils/Structs.h"

// Sets default values
ATD_PlaceablesActors::ATD_PlaceablesActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootMeshComponent  = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootMeshComponent);
	Model = CreateDefaultSubobject<UStaticMeshComponent>("Model");
	Model->SetupAttachment(GetRootComponent());
	Model->SetCollisionProfileName(TEXT("InteractionPreset"));

}

// Called when the game starts or when spawned
void ATD_PlaceablesActors::BeginPlay()
{
	Super::BeginPlay();
	 GameMode = GetWorld()->GetAuthGameMode();
	CurrencyManager= Cast<ATD_GameModeFPS>(GameMode)->CurrencyManagerRef;
	if(Model->GetStaticMesh())
	{
		
		RightSnapPoint = Model->GetStaticMesh()->GetBounds().BoxExtent.Y;
		LeftSnapPoint = RightSnapPoint * -1;
	}
	


}

void ATD_PlaceablesActors::ApplyDamage(float Amount)
{

	CheckIfAlive();
	if(ActorData.Health> ZER0)
	{
		ActorData.Health -= Amount;
		if(ActorData.Health < ZER0)
		{
			RemoveActor();
		}
	}
}

bool ATD_PlaceablesActors::CanEnemiesAttack() const
{
	return (CurrentEnemiesAttackingCount + 1 <= MaxEnemiesWhichCanAttackAtATime);
	
}

void ATD_PlaceablesActors::SetData(FPlaceAbleData Data)
{
	ActorData = Data;
	if (ActorData.PlacementSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ActorData.PlacementSound,this->GetActorLocation());

	}
}

int ATD_PlaceablesActors::GetDestructionCost() const
{
	if(ActorData.GoldCost> ZER0)
	{
		return ActorData.GoldCost / 2;
	}
	return 0;
}

void ATD_PlaceablesActors::RemoveActor()
{
	//TODO do stuff here;
	if(ActorData.DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActorData.DestructionSound, this->GetActorLocation());
	}
	
	if(GameMode)
	{
		bIsAlive = false;
		if(CurrencyManager)
		{
			CurrencyManager->AddCurrency(GetDestructionCost());
		}
		
	}
	this->Destroy();
	/*this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
	this->SetActorTickEnabled(false);
	
	this->SetActorLocation(FVector(1000,1000,-300));*/
	//this->BeginDestroy();
	
}

void ATD_PlaceablesActors::UpgradeActor()
{
	PrintToScreen_Color("Upgrade pressed!", FColor::Blue);

	CheckIfAlive();
	if (ActorData.UpgradeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActorData.UpgradeSound, this->GetActorLocation());
	}
}

void ATD_PlaceablesActors::CheckIfAlive()
{
	if (ActorData.GoldCost < ZER0)
	{
		RemoveActor();
	}
}


// Called every frame
void ATD_PlaceablesActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

