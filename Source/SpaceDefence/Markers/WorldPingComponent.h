// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldPingComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEDEFENCE_API UWorldPingComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Category = "Ping", EditAnywhere)
		UTexture2D* PingTexture;

public:
#pragma region Properties

	UPROPERTY(Category = "Ping", EditAnywhere)
		float PingClearTime;

#pragma endregion
	
	UWorldPingComponent();
	UTexture2D* GetPingTexture();
	float GetPingTime();
};
