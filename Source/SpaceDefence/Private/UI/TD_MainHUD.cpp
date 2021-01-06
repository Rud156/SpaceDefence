// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_MainHUD.h"
#include "DevelopmentTools/TD_DevelopmentTools.h"



void UTD_MainHUD::ShowInteractionMenu(ATD_PlaceablesActors* Actor)
{
	if(InteractionWidgetRef)
	{
		if(InteractionWidgetRef->GetVisibility() != ESlateVisibility::HitTestInvisible)
			InteractionWidgetRef->SetVisibility(ESlateVisibility::HitTestInvisible);

		InteractionWidgetRef->SetInteractingActor(Actor);
	}
	else
	{
		PrintToScreen("No interaction interface added!");
	}
}

void UTD_MainHUD::HideInteractionMenu()
{
	if (InteractionWidgetRef)
	{
		if(InteractionWidgetRef->GetVisibility() != ESlateVisibility::Collapsed)
			InteractionWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
		InteractionWidgetRef->SetInteractingActor(nullptr);
	}
	else
	{
		PrintToScreen("No interaction interface added!");
	}
}
