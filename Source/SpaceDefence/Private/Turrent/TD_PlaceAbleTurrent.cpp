// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceDefence/Public/DevelopmentTools/TD_DevelopmentTools.h"
#include "Turrent/TD_PlaceAbleTurrent.h"



// Sets default values
ATD_PlaceAbleTurrent::ATD_PlaceAbleTurrent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATD_PlaceAbleTurrent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ATD_PlaceAbleTurrent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATD_PlaceAbleTurrent::Fire()
{
	if (Turret)
	{
		
		FVector StartingPoint = Turret->GetSocketLocation(BulletSpawnLocationSocketName);
		FVector ForwardVector = Turret->GetForwardVector();

		FHitResult hitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		
		FRotator SpawnRotation = Turret->GetSocketRotation(BulletSpawnLocationSocketName);
		GetWorld()->SpawnActor<ATD_TurrentProjcetile>(Projectile, StartingPoint, SpawnRotation,SpawnParameters);
		
	}
	else
	{
		PrintToScreen_Color("Turret not set in BP, make sure You set the which component will fire is set in begin play", FColor::Red);
	}
}



