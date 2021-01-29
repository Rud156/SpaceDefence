// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MutantMain.h"
#include "../MutantEnemy.h"
#include "../../Utils/Enums.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_MutantMain::UBTService_MutantMain() : Super()
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
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Mutant Main Became Relevant");

	auto blackboard = OwnerComp.GetBlackboardComponent();
	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	int randomTeleportCount = FMath::RandRange(mutantEnemy->TeleportCountMin, mutantEnemy->TeleportCountMax);

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Value: " + FString::SanitizeFloat(randomTeleportCount));
	blackboard->SetValueAsInt(TeleportCount, randomTeleportCount);

}

void UBTService_MutantMain::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto blackboard = OwnerComp.GetBlackboardComponent();

	int teleportCount = blackboard->GetValueAsInt(TeleportCount);
	EMutantStates currentState = static_cast<EMutantStates>(blackboard->GetValueAsEnum(EnemyState));

	if (teleportCount <= 0 && currentState == EMutantStates::IdleWander)
	{
		blackboard->SetValueAsEnum(EnemyState, static_cast<uint8>(EMutantStates::Attack));
	}

	auto aiController = OwnerComp.GetAIOwner();
	auto mutantEnemy = Cast<AMutantEnemy>(aiController->GetPawn());
	bool isAttackingBB = blackboard->GetValueAsBool(IsAttacking);
	bool isAttackingME = mutantEnemy->GetIsAttacking();

	if (isAttackingBB && !isAttackingME && currentState == EMutantStates::Attack)
	{
		blackboard->SetValueAsEnum(EnemyState, static_cast<uint8>(EMutantStates::Wait));
	}

	blackboard->SetValueAsBool(IsAttacking, isAttackingME);
}