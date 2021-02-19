// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TD_PlayerCompass.generated.h"

/**
 * 
 */

class AFPPlayer;
UCLASS()
class SPACEDEFENCE_API UTD_PlayerCompass : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFPPlayer* PlayerRef = nullptr;
};
