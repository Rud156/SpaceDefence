// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldPingMarker.generated.h"

class UWidgetComponent;

UCLASS()
class SPACEDEFENCE_API AWorldPingMarker : public AActor
{
	GENERATED_BODY()

	void UpdateWidgetRotation(float deltaTime);
	void UpdateWidgetDistance(float deltaTime);
	void UpdateWidgetDestroyTimer(float deltaTime);

	float _currentTime;

protected:
	virtual void BeginPlay() override;

public:
#pragma region Properties
	
	UPROPERTY(Category = Widget, VisibleDefaultsOnly, BlueprintReadOnly)
		UWidgetComponent* PingWidget;

	UPROPERTY(Category = Widget, VisibleDefaultsOnly)
		USceneComponent* PingRoot;

	UPROPERTY(Category = "Ping", EditAnywhere)
		float DefaultPingTime;

#pragma endregion

	AWorldPingMarker();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Event", BlueprintImplementableEvent)
		void EventUpdateWidgetDistance(float distance);

	void UpdateWidgetTexture(UTexture2D* texture);
	UFUNCTION(Category = "Event", BlueprintImplementableEvent)
		void EventUpdateWidgetTexture(UTexture2D* texture);

	void SetPingTime(float time);
};
