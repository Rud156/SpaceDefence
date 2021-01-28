// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MutantMain.generated.h"

UCLASS()
class SPACEDEFENCE_API UBTService_MutantMain : public UBTService
{
	GENERATED_BODY()

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName EnemyState;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName TeleportCount;

#pragma endregion

	UBTService_MutantMain();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
