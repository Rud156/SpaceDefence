// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectPointInRadius.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_SelectPointInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	auto actorLocation = aiController->GetPawn()->GetActorLocation();

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation navLocation;
	bool success = navSystem->GetRandomPointInNavigableRadius(actorLocation, PointInRadius, navLocation);

	if (success)
	{

		auto blackboard = OwnerComp.GetBlackboardComponent();
		blackboard->SetValueAsVector(MovementPoint, navLocation.Location);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Failed");
	}

	return EBTNodeResult::Succeeded;
}