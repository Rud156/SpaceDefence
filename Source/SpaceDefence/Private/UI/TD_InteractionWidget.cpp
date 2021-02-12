// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceDefence/Public/UI/TD_InteractionWidget.h"

void UTD_InteractionWidget::OnUpgradePressed()
{
	if(CurrentInteractingActor)
	{
		CurrentInteractingActor->UpgradeActor();
	}
}

void UTD_InteractionWidget::OnRemovePressed()
{
	if (CurrentInteractingActor)
	{
		CurrentInteractingActor->RemoveActor(nullptr);
	}
}
