// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlaceablesActors.h"
#include "SpaceDefence/Public/DevelopmentTools/TD_DevelopmentTools.h"
#include "TD_GameModeFPS.h"
#include "CurrencyManager/TD_CurrencyManager.h"
#include "SpaceDefence/Public/TD_GameStateBase.h"
#include "Kismet/GameplayStatics.h"

#define ZER0 0.0

ATD_PlaceablesActors::ATD_PlaceablesActors()
{
	PrimaryActorTick.bCanEverTick = false;
	RootMeshComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootMeshComponent);
	Model = CreateDefaultSubobject<UStaticMeshComponent>("Model");
	Model->SetupAttachment(GetRootComponent());
	Model->SetCollisionProfileName(TEXT("InteractionPreset"));
	HealthAndDamage = CreateDefaultSubobject<UHealthAndDamageComp>("Health and Damage comp");

}

// Called when the game starts or when spawned
void ATD_PlaceablesActors::BeginPlay()
{
	Super::BeginPlay();
	if (HealthAndDamage)
	{
		HealthAndDamage->OnUnitDied.AddDynamic(this, &ATD_PlaceablesActors::RemoveActor);
		HealthAndDamage->SetMaxHealth(ActorData.Health);

	}
	GameMode = GetWorld()->GetAuthGameMode();
	GameSate = Cast<ATD_GameStateBase>(GetWorld()->GetGameState());
	//CurrencyManager = Cast<ATD_GameModeFPS>(GameMode)->CurrencyManagerRef;
	if (Model->GetStaticMesh())
	{

		RightSnapPoint = Model->GetStaticMesh()->GetBounds().BoxExtent.Y;
		LeftSnapPoint = RightSnapPoint * -1;
	}
}

void ATD_PlaceablesActors::TakeDamage(float Amount)
{
	CheckIfAlive();

	if (HealthAndDamage->GetCurrentHealth() > ZER0)
	{
		HealthAndDamage->TakeDamage(Amount);
		ActorData.Health = HealthAndDamage->GetCurrentHealth();
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

		if (HealthAndDamage)
		{
			HealthAndDamage->SetMaxHealth(ActorData.Health);

		}
		UGameplayStatics::PlaySoundAtLocation(this, ActorData.PlacementSound, this->GetActorLocation());

	}
}

int ATD_PlaceablesActors::GetDestructionCost() const
{
	if (ActorData.GoldCost > ZER0)
	{
		return ActorData.GoldCost / 2;
	}
	return 0;
}

void ATD_PlaceablesActors::RemoveActor(AActor* Actor)
{
	//TODO do stuff here;
	if (ActorData.DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActorData.DestructionSound, this->GetActorLocation());
	}

	if (GameMode)
	{
		bIsAlive = false;
		//TODO fix here
		/*if (CurrencyManager)
		{
			CurrencyManager->AddCurrency(GetDestructionCost());
		}*/

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
		RemoveActor(nullptr);
	}
}


// Called every frame
void ATD_PlaceablesActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

