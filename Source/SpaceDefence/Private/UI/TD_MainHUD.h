// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TD_MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UTD_MainHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	

	UPROPERTY(VisibleAnywhere,Category = "Game Play variables", BlueprintReadWrite)
		TArray<UUserWidget*>	ListOfQuickSlotItems;

	UPROPERTY(EditAnywhere, Category = "Game Play variables", BlueprintReadWrite)
		float NormalModeYSlotPosition = -100;
	UPROPERTY(EditAnywhere, Category = "Game Play variables", BlueprintReadWrite)
		float InventoryModeYSlotPosition = -172;

	UPROPERTY(EditAnywhere, Category = "Game Play variables", BlueprintReadWrite)
		bool bIsInventoryToggled = false;
		

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ToggleQuickSlotPosition();

	
private:
	//game play variables.


};
