// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionDisplayManager.generated.h"

UCLASS()
class SPACEDEFENCE_API AInteractionDisplayManager : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AInteractionDisplayManager();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = Display, BlueprintImplementableEvent)
		void ShowInteractionBar();

	UFUNCTION(Category = Display, BlueprintImplementableEvent)
		void HideInteractionBar();

	UFUNCTION(Category = Display, BlueprintImplementableEvent)
		void SetInteractionBarProgress(float progress);
};
