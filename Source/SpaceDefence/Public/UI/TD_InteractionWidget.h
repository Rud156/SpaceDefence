// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceDefence/Placeables/TD_PlaceablesActors.h"

#include "TD_InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEDEFENCE_API UTD_InteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
		UPROPERTY(BlueprintReadWrite)
		ATD_PlaceablesActors* CurrentInteractingActor;





		//functions
		void SetInteractingActor(ATD_PlaceablesActors* ActorRef) { CurrentInteractingActor = ActorRef; }

		void OnUpgradePressed();
		void OnRemovePressed();
	

private:

	
	
	
};
