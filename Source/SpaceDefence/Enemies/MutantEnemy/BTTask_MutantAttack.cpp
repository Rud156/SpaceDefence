// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MutantAttack.h"
#include "../MutantEnemy.h"
#include "../../Utils/Enums.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MutantAttack::UBTTask_MutantAttack()
{
	EnemyState = "Enemy State";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MutantAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());

	_currentWaitTime = mutantEnemy->Attack();
	return EBTNodeResult::InProgress;
}

void UBTTask_MutantAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	_currentWaitTime -= DeltaSeconds;
	if (_currentWaitTime <= 0)
	{
		auto blackboardComponent = OwnerComp.GetBlackboardComponent();
		blackboardComponent->SetValueAsEnum(EnemyState, static_cast<uint8>(EMutantStates::Wait));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
