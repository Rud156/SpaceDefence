// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPingComponent.h"


UWorldPingComponent::UWorldPingComponent()
{
	// Uncomment this if required later on...
	// PrimaryComponentTick.bCanEverTick = true;
}

UTexture2D* UWorldPingComponent::GetPingTexture()
{
	return  PingTexture;
}

float UWorldPingComponent::GetPingTime()
{
	return PingClearTime;
}