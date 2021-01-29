// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MutantAttack.generated.h"

/**
 *
 */
UCLASS()
class SPACEDEFENCE_API UBTTask_MutantAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName EnemyState;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName IsAttacking;

#pragma endregion

	UBTTask_MutantAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
