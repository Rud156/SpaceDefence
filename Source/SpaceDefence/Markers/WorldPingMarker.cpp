// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPingMarker.h"

#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AWorldPingMarker::AWorldPingMarker()
{
	PingRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PingRoot"));
	PingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PingWidget"));

	RootComponent = PingRoot;
	PingWidget->SetupAttachment(PingRoot);

	PrimaryActorTick.bCanEverTick = true;
}

void AWorldPingMarker::BeginPlay()
{
	Super::BeginPlay();

	if (_currentTime == 0)
	{
		_currentTime = DefaultPingTime;
	}
}

void AWorldPingMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWidgetRotation(DeltaTime);
	UpdateWidgetDistance(DeltaTime);
	UpdateWidgetDestroyTimer(DeltaTime);
}

void AWorldPingMarker::UpdateWidgetRotation(float deltaTime)
{
	auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector widgetLocation = GetActorLocation();
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(widgetLocation, cameraLocation);

	SetActorRotation(lookRotation);
}

void AWorldPingMarker::UpdateWidgetDistance(float deltaTime)
{
	auto cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector cameraLocation = cameraManager->GetCameraLocation();

	float distance = FVector::Distance(cameraLocation, GetActorLocation());
	EventUpdateWidgetDistance(distance);
}

void AWorldPingMarker::UpdateWidgetDestroyTimer(float deltaTime)
{
	if (_currentTime > 0)
	{
		_currentTime -= deltaTime;
		if (_currentTime <= 0)
		{
			Destroy();
		}
	}
}

void AWorldPingMarker::UpdateWidgetTexture(UTexture2D* texture)
{
	EventUpdateWidgetTexture(texture);
}

void AWorldPingMarker::SetPingTime(float time)
{
	_currentTime = time;
}