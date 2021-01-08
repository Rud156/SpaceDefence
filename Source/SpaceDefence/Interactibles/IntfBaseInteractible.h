// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../Utils/Enums.h"

#include "IntfBaseInteractible.generated.h"

UINTERFACE(BlueprintType)
class SPACEDEFENCE_API UIntfBaseInteractible : public UInterface
{
	GENERATED_BODY()
};


class SPACEDEFENCE_API IIntfBaseInteractible
{
	GENERATED_BODY()
	
public:

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		EInteractibleType GetInteractibleType();
	
	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		bool InteractUpdate(float DeltaTime);

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		void CancelInteraction();

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		bool InteractionStarted();

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		float GetCurrentInteractionTime();

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		float GetBaseInteractionTime();

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		float GetInteractionProgress();

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintNativeEvent)
		void SetInteractionTime(float DurationMultiplier);
};