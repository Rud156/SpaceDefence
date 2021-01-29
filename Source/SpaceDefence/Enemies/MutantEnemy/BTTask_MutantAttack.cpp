// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MutantAttack.h"
#include "../MutantEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MutantAttack::UBTTask_MutantAttack() : Super()
{
	EnemyState = "Enemy State";
	IsAttacking = "Is Attacking";

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MutantAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	auto blackboard = OwnerComp.GetBlackboardComponent();

	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	blackboard->SetValueAsBool(IsAttacking, mutantEnemy->GetIsAttacking());

	mutantEnemy->Attack();
	return EBTNodeResult::Succeeded;
}
