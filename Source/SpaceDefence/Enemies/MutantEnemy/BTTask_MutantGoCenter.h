// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MutantGoCenter.generated.h"

UCLASS()
class SPACEDEFENCE_API UBTTask_MutantGoCenter : public UBTTaskNode
{
	GENERATED_BODY()

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName MovementPoint;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		float SearchRadius;

#pragma endregion

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
