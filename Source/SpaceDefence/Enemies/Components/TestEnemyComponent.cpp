// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemyComponent.h"

UTestEnemyComponent::UTestEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTestEnemyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTestEnemyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UTestEnemyComponent::TestCustomComponent()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Hello World From Custom Component");
	return 	FMath::RandRange(1, 10);
}