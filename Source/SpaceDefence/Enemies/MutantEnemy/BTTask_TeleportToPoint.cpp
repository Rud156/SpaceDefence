// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TeleportToPoint.h"
#include "../MutantEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TeleportToPoint::UBTTask_TeleportToPoint() : Super()
{
	MovementPoint = "Movement Point";
	TeleportCount = "Teleport Count";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TeleportToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	int teleportStartTime = mutantEnemy->TeleportStart();

	_currentTime = teleportStartTime;
	_isStarting = true;

	return EBTNodeResult::InProgress;
}

void UBTTask_TeleportToPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	_currentTime -= DeltaSeconds;

	if (_currentTime <= 0)
	{
		if (_isStarting)
		{
			auto blackboard = OwnerComp.GetBlackboardComponent();
			auto aiController = OwnerComp.GetAIOwner();
			auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());

			int currentTeleportCount = blackboard->GetValueAsInt(TeleportCount);
			currentTeleportCount -= 1;
			blackboard->SetValueAsInt(TeleportCount, currentTeleportCount);

			FVector backVector = -mutantEnemy->GetActorForwardVector();
			FVector movementPoint = blackboard->GetValueAsVector(MovementPoint);

			FVector targetPoint = movementPoint + backVector * TeleportNearestDistance;

			FRotator currentRotation = mutantEnemy->GetActorRotation();
			bool success = mutantEnemy->TeleportTo(targetPoint, currentRotation);

			if (!success)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Teleport Failed...");
			}

			int teleportEndTime = mutantEnemy->TeleportEnd();

			_isStarting = false;
			_currentTime = teleportEndTime;
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}