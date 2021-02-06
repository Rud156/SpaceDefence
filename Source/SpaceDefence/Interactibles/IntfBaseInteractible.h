// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IntfBaseInteractible.generated.h"

class UInteractionComponent;

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
		UInteractionComponent* GetInteractionComponent();
};