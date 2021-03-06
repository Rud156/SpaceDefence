// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Utils/Enums.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionComplete);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEDEFENCE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool _interactionStarted;
	float _interactionDuration;
	float _currentInteractionTime;

protected:
	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Interaction|Data", EditAnywhere)
		float BaseDuration;

	UPROPERTY(Category = "Interaction|Data", EditAnywhere)
		EInteractibleType InteractibleType;

	UPROPERTY(Category = "Delegates", BlueprintAssignable)
		FInteractionComplete OnInteractionComplete;

#pragma endregion

	UInteractionComponent();
	bool InteractionUpdate(float deltaTime);

	EInteractibleType GetInteractibleType();
	void StartInteraction();
	void CancelInteraction();
	void SetInteractionTime(float durationMultiplier = 1);

	float GetCurrentInteractionTime();
	float GetBaseInteractionTime();
	float GetInteractionProgress();
	bool IsInteractionActive();
};
