// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TeleportToPoint.h"
#include "../MutantEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TeleportToPoint::UBTTask_TeleportToPoint()
{
	TeleportStartTime = 1.0f;
	TeleportEndTime = 1.5f;
	MovementPoint = "Movement Point";
	TeleportCount = "Teleport Count";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TeleportToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	mutantEnemy->TeleportStart();

	_currentTime = TeleportStartTime;
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
			_isStarting = false;
			_currentTime = TeleportEndTime;

			auto blackboard = OwnerComp.GetBlackboardComponent();
			auto aiController = OwnerComp.GetAIOwner();
			auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());

			int currentTeleportCount = blackboard->GetValueAsInt(TeleportCount);
			currentTeleportCount -= 1;
			blackboard->SetValueAsInt(TeleportCount, currentTeleportCount);

			FVector movementPoint = blackboard->GetValueAsVector(MovementPoint);

			mutantEnemy->SetActorLocation(movementPoint);
			mutantEnemy->TeleportEnd();

			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Teleporting Complete");
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}