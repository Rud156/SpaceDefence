// Fill out your copyright notice in the Description page of Project Settings.


#include "./BTTask_MutantGoCenter.h"

#include "Engine/TargetPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_MutantGoCenter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto aiController = OwnerComp.GetAIOwner();
	AActor* mapCenter = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetPoint::StaticClass());
	FVector targetLocation = aiController->GetPawn()->GetActorLocation();
	if (mapCenter != nullptr)
	{
		targetLocation = mapCenter->GetActorLocation();
	}

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation navLocation;

	bool success = navSystem->GetRandomPointInNavigableRadius(targetLocation, SearchRadius, navLocation);
	auto blackboard = OwnerComp.GetBlackboardComponent();

	if (success)
	{
		blackboard->SetValueAsVector(MovementPoint, navLocation.Location);
	}
	else
	{
		auto actorLocation = aiController->GetPawn()->GetActorLocation();
		blackboard->SetValueAsVector(MovementPoint, actorLocation);
	}

	return EBTNodeResult::Succeeded;
}