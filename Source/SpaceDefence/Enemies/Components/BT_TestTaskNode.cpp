// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_TestTaskNode.h"
#include "../BaseEnemy.h"
#include "TestEnemyComponent.h"

#include "AIController.h"

EBTNodeResult::Type UBT_TestTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	if (aiController != nullptr)
	{
		auto pawn = aiController->GetPawn();
		ABaseEnemy* baseEnemy = Cast<ABaseEnemy>(pawn);

		if (baseEnemy != nullptr)
		{
			auto testComponentActor = baseEnemy->GetComponentByClass(UTestEnemyComponent::StaticClass());
			UTestEnemyComponent* testComponent = Cast<UTestEnemyComponent>(testComponentActor);

			int result = testComponent->TestCustomComponent();
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Returned Value: " + FString::SanitizeFloat(result));
		}
	}

	return EBTNodeResult::Succeeded;
}

void UBT_TestTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Task Ticking: " + FString::SanitizeFloat(DeltaSeconds));
}