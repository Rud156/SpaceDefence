// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

UCLASS()
class SPACEDEFENCE_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	void HandleCharacterJump();
	void HandleRunPressed();
	void HandleRunReleased();
	void HandleCrouchPressed();
	void HandleCrouchReleased();
	void HandleFirePressed();
	void HandleFireReleased();
	void HandleInteractPressed();
	void HandleInteractReleased();
	void HandleMeleeSelected();
	void HandlePrimarySelected();
	void HandleSecondarySelected();
	void HandleTestCheckAndDropWeapon();
	void HandlePlayerPinged();

	void MoveForward(float input);
	void MoveRight(float input);
	void Turn(float input);
	void LookUp(float input);

protected:
	virtual void SetupInputComponent() override;
};
