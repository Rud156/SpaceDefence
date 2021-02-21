// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TD_GameModeFPS.generated.h"

UCLASS()
class ATD_GameModeFPS : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "References", BlueprintReadWrite)
		class UUserWidget* InventoryBPRef;

	UPROPERTY(Category = "References", BlueprintReadWrite)
		class UTD_MainHUD* MainHUDRef;

	/*UPROPERTY(Category = "References", BlueprintReadWrite)
		class ATD_CurrencyManager* CurrencyManagerRef;*/

	UPROPERTY(Category = "References", BlueprintReadWrite)
		class ATD_WaveController* WaveControllerRef;

	UPROPERTY(Category = "References", BlueprintReadWrite)
		class AInteractionDisplayManager* InteractionDisplayManagerRef;
};
