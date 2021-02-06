// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInteractionComponent::InteractionUpdate(float deltaTime)
{
	if (!_interactionStarted)
	{
		return false;
	}

	_currentInteractionTime -= deltaTime;
	if (_currentInteractionTime <= 0)
	{
		_interactionStarted = false;
		OnInteractionComplete.Broadcast();

		return true;
	}

	return false;
}

EInteractibleType UInteractionComponent::GetInteractibleType()
{
	return InteractibleType;
}

void UInteractionComponent::StartInteraction()
{
	_interactionStarted = true;
}

void UInteractionComponent::CancelInteraction()
{
	_interactionStarted = false;
	_currentInteractionTime = 0;
	_interactionDuration = 0;
}

void UInteractionComponent::SetInteractionTime(float durationMultiplier)
{
	_interactionDuration = BaseDuration * durationMultiplier;
	_currentInteractionTime = _interactionDuration;
}

float UInteractionComponent::GetCurrentInteractionTime()
{
	return _currentInteractionTime;
}

float UInteractionComponent::GetBaseInteractionTime()
{
	return _interactionDuration;
}

float UInteractionComponent::GetInteractionProgress()
{
	return 1 - (_currentInteractionTime / _interactionDuration);
}

bool UInteractionComponent::IsInteractionActive()
{
	return _interactionStarted;
}