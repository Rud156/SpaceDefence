// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DefenceBuffBaseComp.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEDEFENCE_API UDefenceBuffBaseComp : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UDefenceBuffBaseComp();
	
	// Returns the DeBUffed Version of the damage
	virtual int TakeDamage(int damageAmount);
};
