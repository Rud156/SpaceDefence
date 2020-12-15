// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlaceableSpawnerActor.h"

// Sets default values
ATD_PlaceableSpawnerActor::ATD_PlaceableSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootMeshComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootMeshComponent);
	Ghost = CreateDefaultSubobject<UStaticMeshComponent>("Model");
	Ghost->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATD_PlaceableSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATD_PlaceableSpawnerActor::SpawnPlaceAbleGhost(FVector Location, int PlaceAbleID)
{
	if (GetWorld())
	{

		Ghost->SetStaticMesh(GetStaticMeshFromList(PlaceAbleID));

		

		//Ghost->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);

	}
	
	
}

void ATD_PlaceableSpawnerActor::SpawnActorFromGhost( int PlaceAbleID)
{
	if (GetWorld() )
	{
		if(CanPlace())
		{
			
		FRotator Rot;
		auto Location = Ghost->GetComponentLocation();
		ActorRef = Cast<ATD_PlaceablesActors>(GetWorld()->SpawnActor(PlaceAbleData[PlaceAbleID].ActorRef,&Location, &Rot));
		}

		RemoveGhost();
		
		//Ghost->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);

	}
}

void ATD_PlaceableSpawnerActor::RemoveGhost()
{
	Ghost->SetMaterial(0, nullptr);
	Ghost->SetStaticMesh(nullptr);
}

void ATD_PlaceableSpawnerActor::SetGhostPosition(FVector Location)
{
	FTransform Temp;
	Temp.SetLocation(Location);
	Temp.SetScale3D(FVector(1, 1, 1));
	Ghost->SetWorldTransform(Temp);
	ChangeColor();
}

void ATD_PlaceableSpawnerActor::ChangeColor()
{
	if(CanPlace())
	{
		Ghost->SetMaterial(0, GreenColor);
	}
	else
	{
		Ghost->SetMaterial(0, RedColor);
	}
}

UStaticMesh* ATD_PlaceableSpawnerActor::GetStaticMeshFromList(int PlaceAbleID)
{
	const auto temp = PlaceAbleData[PlaceAbleID].ActorRef.GetDefaultObject()->Model->GetStaticMesh();
	
	return temp;
}

bool ATD_PlaceableSpawnerActor::CanPlace()
{
	
	return (!bIsColliding && bIsOnGround);
}

bool ATD_PlaceableSpawnerActor::CheckIsOnGround()
{
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Line Trace")), true, this);
	FHitResult Hit(ForceInit);
	if(GetWorld() && Ghost->GetStaticMesh())
	{
		auto X= Ghost->GetStaticMesh()->GetBounds().BoxExtent.X/2;

		FVector Start= FVector(Ghost->GetComponentLocation().X-X, Ghost->GetComponentLocation().Y, Ghost->GetComponentLocation().Z);
		FVector End = FVector(Start.X, Start.Y, Start.Z - MinGroundCheckDistance);
	
		GetWorld()->LineTraceSingleByChannel(Hit,Start,End,ECollisionChannel::ECC_Visibility);

		return Hit.bBlockingHit;
	}
	return false;
}


// Called every frame
void ATD_PlaceableSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Ghost!=nullptr)
	{
		bIsOnGround = CheckIsOnGround();
	}
}

