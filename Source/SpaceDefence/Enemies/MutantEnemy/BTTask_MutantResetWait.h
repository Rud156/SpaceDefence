// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MutantResetWait.generated.h"

/**
 *
 */
UCLASS()
class SPACEDEFENCE_API UBTTask_MutantResetWait : public UBTTaskNode
{
	GENERATED_BODY()

private:
	float _currentWait;

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		float WaitTime;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		float RandomDeviation;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName EnemyState;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName TeleportCount;

#pragma endregion

	UBTTask_MutantResetWait();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
