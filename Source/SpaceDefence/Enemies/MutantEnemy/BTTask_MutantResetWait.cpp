// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MutantResetWait.h"
#include "../MutantEnemy.h"
#include "../../Utils/Enums.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MutantResetWait::UBTTask_MutantResetWait() : Super()
{
	EnemyState = "Enemy State";
	TeleportCount = "Teleport Count";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MutantResetWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	_currentWait = WaitTime + FMath::RandRange(-RandomDeviation, RandomDeviation);
	return EBTNodeResult::InProgress;
}

void UBTTask_MutantResetWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	_currentWait -= DeltaSeconds;
	if (_currentWait <= 0)
	{
		auto blackboard = OwnerComp.GetBlackboardComponent();
		blackboard->SetValueAsEnum(EnemyState, static_cast<uint8>(EMutantState::IdleWander));

		auto aiController = OwnerComp.GetAIOwner();
		auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
		int randomTeleportCount = FMath::RandRange(mutantEnemy->TeleportCountMin, mutantEnemy->TeleportCountMax);
		blackboard->SetValueAsInt(TeleportCount, randomTeleportCount);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}