// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TD_GameModeFPS.generated.h"

/**
 * 
 */
UCLASS()
class ATD_GameModeFPS : public AGameModeBase
{
	GENERATED_BODY()


		
public:


public:
	UPROPERTY(VisibleAnywhere, Category="References",BlueprintReadWrite)
		class UUserWidget* InventoryBPRef;


protected:
	

	
};
