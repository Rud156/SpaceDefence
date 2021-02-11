// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MutantMain.h"
#include "../MutantEnemy.h"
#include "../../Utils/Enums.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_MutantMain::UBTService_MutantMain()
{
	EnemyState = "Enemy State";
	TeleportCount = "Teleport Count";
	IsAttacking = "Is Attacking";

	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyOnSearch = true;
}

void UBTService_MutantMain::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto blackboard = OwnerComp.GetBlackboardComponent();
	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	int randomTeleportCount = FMath::RandRange(mutantEnemy->TeleportCountMin, mutantEnemy->TeleportCountMax);

	blackboard->SetValueAsInt(TeleportCount, randomTeleportCount);
}

void UBTService_MutantMain::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto blackboard = OwnerComp.GetBlackboardComponent();

	int teleportCount = blackboard->GetValueAsInt(TeleportCount);
	EMutantState currentState = static_cast<EMutantState>(blackboard->GetValueAsEnum(EnemyState));

	if (teleportCount <= 0 && currentState == EMutantState::IdleWander)
	{
		blackboard->SetValueAsEnum(EnemyState, static_cast<uint8>(EMutantState::Attack));
	}

	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	bool isAttackingBB = blackboard->GetValueAsBool(IsAttacking);
	bool isAttackingME = mutantEnemy->GetIsAttacking();

	if (isAttackingBB && !isAttackingME && currentState == EMutantState::Attack)
	{
		blackboard->SetValueAsEnum(EnemyState, static_cast<uint8>(EMutantState::Wait));
	}

	blackboard->SetValueAsBool(IsAttacking, isAttackingME);
}