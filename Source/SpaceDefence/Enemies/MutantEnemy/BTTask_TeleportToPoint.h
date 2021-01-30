// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TeleportToPoint.generated.h"

UCLASS()
class SPACEDEFENCE_API UBTTask_TeleportToPoint : public UBTTaskNode
{
	GENERATED_BODY()

private:
	float _currentTime;
	bool _isStarting;

public:
#pragma region Properties

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName MovementPoint;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		FName TeleportCount;

	UPROPERTY(Category = "Enemy|AI", EditAnywhere)
		float TeleportNearestDistance;

#pragma endregion

	UBTTask_TeleportToPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
