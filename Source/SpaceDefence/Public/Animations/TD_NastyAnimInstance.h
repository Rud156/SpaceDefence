// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TD_NastyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEDEFENCE_API UTD_NastyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,CallInEditor)
		void TestSocket();
};
