// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlaceableSpawnerActor.h"
#include "DrawDebugHelpers.h"
#include "TD_GameModeFPS.h"
#include "SpaceDefence/Public/DevelopmentTools/TD_DevelopmentTools.h"
#include "CurrencyManager/TD_CurrencyManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATD_PlaceableSpawnerActor::ATD_PlaceableSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootMeshComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootMeshComponent);
	Ghost = CreateDefaultSubobject<UStaticMeshComponent>("Model");
	Ghost->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UDataTable> PlaceAbleDataTableObject(TEXT("DataTable'/Game/Data/PlaceAbleActorData.PlaceAbleActorData'"));
	if (PlaceAbleDataTableObject.Succeeded())
	{
		PlaceAbleDataTable = PlaceAbleDataTableObject.Object;
	}
	if (PlaceAbleDataTable)
	{
		auto Names = PlaceAbleDataTable->GetRowNames();
		const FString ContextString;
		for (const auto RowName : Names)
		{
			FPlaceAbleData* tempData = PlaceAbleDataTable->FindRow<FPlaceAbleData>(RowName, ContextString);
			if(tempData)
				PlaceAbleData.Add(*tempData);
			else
			{
				PrintToScreen_Color("Error in the placable data table rows", FColor::Red);
			}
		}
	}
	
}

// Called when the game starts or when spawned
void ATD_PlaceableSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	auto GameMode = GetWorld()->GetAuthGameMode();
	CurrencyRef = Cast<ATD_GameModeFPS>(GameMode)->CurrencyManagerRef;
	
}

bool ATD_PlaceableSpawnerActor::CanSpawnGhost(int Cost)
{	
		if (CurrencyRef)
			if (Cast<ATD_CurrencyManager>(CurrencyRef)->HasCurrency(Cost))
			{
				return true;
			}
	
	
	return false;
}

int ATD_PlaceableSpawnerActor::GetCostFromID(int PlaceAbleID)
{
	return PlaceAbleData[PlaceAbleID].GoldCost;
}

bool ATD_PlaceableSpawnerActor::SpawnPlaceAbleGhost(FVector Location, int PlaceAbleID)
{
	if (GetWorld()&& PlaceAbleID>-1 && PlaceAbleID<9)
	{


		if(CanSpawnGhost(GetCostFromID(PlaceAbleID)))
		{
			
			Ghost->SetStaticMesh(GetStaticMeshFromList(PlaceAbleID));
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Not Enough Gold Print UI here")));
			Ghost->SetStaticMesh(nullptr);
			return false;
		}

		

		//Ghost->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);

	}
	

	return false;
	
}

void ATD_PlaceableSpawnerActor::ChangeGhost(int PlaceAbleID)
{
	if (GetWorld() && PlaceAbleID > -1 && PlaceAbleID < 9)
	{
		if(CanSpawnGhost(GetCostFromID(PlaceAbleID)))
			Ghost->SetStaticMesh(GetStaticMeshFromList(PlaceAbleID));
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Not Enough Gold Print UI here %d"), CurrencyRef->GetCurrency()));
		}
	}
}

void ATD_PlaceableSpawnerActor::SpawnActorFromGhost( int PlaceAbleID)
{
	if (GetWorld() )
	{
		if(CanPlace())
		{
			if(Ghost->GetStaticMesh())
			{
				
				FRotator Rot = Ghost->GetComponentRotation();
				auto Location = Ghost->GetComponentLocation();
				CurrencyRef->RemoveCurrency(GetCostFromID(PlaceAbleID));
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Not Enough Gold Print UI here %d"),CurrencyRef->GetCurrency()));
				ActorRef = Cast<ATD_PlaceablesActors>(GetWorld()->SpawnActor(PlaceAbleData[PlaceAbleID].ActorRef,&Location, &Rot));
				//TODO: Check optimization here. 
				ActorRef->SetData(PlaceAbleData[PlaceAbleID]);
			}
		}

		RemoveGhost();
		
		//Ghost->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);

	}
}

void ATD_PlaceableSpawnerActor::RemoveGhost()
{
	for (int i =0; i< Ghost->GetNumMaterials(); i++)
	{
		Ghost->SetMaterial(i, nullptr);
	}
	
	Ghost->SetStaticMesh(nullptr);
	bIsRotated = false;
}

void ATD_PlaceableSpawnerActor::SetGhostPosition(FVector Location, FRotator Rot= FRotator(0,0,0), FVector Offset= FVector(0,0,0))
{
	FTransform Temp;
	Temp.SetLocation(Location);
	//FQuat TempRot = FQuat(Rot.Quaternion().X,0, Rot.Quaternion().Z,Rot.Quaternion().W);
	//Temp.SetRotation(Rot.Quaternion());
	
	Temp.SetScale3D(Ghost->GetComponentScale());
	Temp.SetRotation(Ghost->GetComponentRotation().Quaternion());
	Ghost->SetWorldTransform(Temp);
	
	if(bIsRotated)
	{
		Rot = Ghost->GetComponentRotation();
		//Ghost->SetWorldRotation(Rot);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("here")));
	}else
	{
		Ghost->SetWorldRotation(Rot);
		
	}
	Ghost->AddLocalOffset(Offset);
	ChangeColor();
}

void ATD_PlaceableSpawnerActor::ChangeColor()
{
	if(CanPlace())
	{
		for(int i =0 ;i< Ghost->GetNumMaterials();i++)
			Ghost->SetMaterial(i, GreenColor);
	}
	else
	{
		for (int i = 0; i < Ghost->GetNumMaterials(); i++)
			Ghost->SetMaterial(i, RedColor);
		
	}
}

UStaticMesh* ATD_PlaceableSpawnerActor::GetStaticMeshFromList(int PlaceAbleID)
{
	UStaticMesh* temp = PlaceAbleData[PlaceAbleID].TempStaticMesh;
	if(!temp)
	{
		temp = PlaceAbleData[PlaceAbleID].ActorRef.GetDefaultObject()->Model->GetStaticMesh();	
	}
	Ghost->SetWorldScale3D(PlaceAbleData[PlaceAbleID].ActorRef.GetDefaultObject()->Model->GetRelativeScale3D());
	return temp;
}

bool ATD_PlaceableSpawnerActor::CanPlace()
{
	
	return (!bIsColliding && bIsOnGround);
}

void ATD_PlaceableSpawnerActor::RotateLeft(FRotator Rot)
{
	bIsRotated = true;
	Ghost->AddWorldRotation(Rot);
}

void ATD_PlaceableSpawnerActor::RotateRight(FRotator Rot)
{
	bIsRotated = true;
	
	Ghost->AddWorldRotation(Rot);
}

bool ATD_PlaceableSpawnerActor::CheckIsOnGround()
{
	/*FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("Line Trace")), true, this);
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	CollisionParams.TraceTag = TraceTag;
	FHitResult Hit;
	if(GetWorld() && Ghost->GetStaticMesh())
	{
		FVector Min, Max;
		Ghost->GetLocalBounds(Min, Max);

		FVector Start= FVector(Ghost->GetComponentLocation().X, Ghost->GetComponentLocation().Y, Ghost->GetComponentLocation().Z);
		
		FVector End = FVector(Start.X, Start.Y, Start.Z - MinGroundCheckDistance);
		
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
		GetWorld()->LineTraceSingleByChannel(Hit,Start,End,ECollisionChannel::ECC_Visibility);
		if(Hit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,FString::Printf(TEXT("Sir you are hitting ground %s"),*Hit.GetActor()->GetName()));
		}
		return Hit.bBlockingHit;
	}*/
	return false;
}


// Called every frame
void ATD_PlaceableSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

