// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TD_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEDEFENCE_API ATD_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()


public:


	UPROPERTY(Category = "References", BlueprintReadWrite)
		class UTD_MainHUD* MainHUDRef;
	
	UPROPERTY(Category = "References", BlueprintReadWrite)
		class ATD_CurrencyManager* CurrencyManagerRef;
};
