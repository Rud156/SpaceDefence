// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerController.h"
#include "FPPlayer.h"

void AFPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleCharacterJump);
	InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleRunPressed);
	InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AFPPlayerController::HandleRunReleased);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleCrouchPressed);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AFPPlayerController::HandleCrouchReleased);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleFirePressed);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AFPPlayerController::HandleFireReleased);
	InputComponent->BindAction("AltFire", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleAltFirePressed);
	InputComponent->BindAction("AltFire", EInputEvent::IE_Released, this, &AFPPlayerController::HandleAltFireReleased);
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleInteractPressed);
	InputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &AFPPlayerController::HandleInteractReleased);
	InputComponent->BindAction("Melee", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleMeleeSelected);
	InputComponent->BindAction("Primary", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandlePrimarySelected);
	InputComponent->BindAction("Secondary", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandleSecondarySelected);
	InputComponent->BindAction("TestDropCurrentWeapon", IE_Pressed, this, &AFPPlayerController::HandleTestCheckAndDropWeapon);
	InputComponent->BindAction("Ping", EInputEvent::IE_Pressed, this, &AFPPlayerController::HandlePlayerPinged);

	InputComponent->BindAxis("MoveForward", this, &AFPPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AFPPlayerController::LookUp);
}

void AFPPlayerController::HandleCharacterJump()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->CharacterJump();
}

void AFPPlayerController::HandleRunPressed()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->RunPressed();
}

void AFPPlayerController::HandleRunReleased()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->RunReleased();
}

void AFPPlayerController::HandleCrouchPressed()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->CrouchPressed();
}

void AFPPlayerController::HandleCrouchReleased()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->CrouchReleased();
}

void AFPPlayerController::HandleFirePressed()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->FirePressed();
}

void AFPPlayerController::HandleFireReleased()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->FireReleased();
}

void AFPPlayerController::HandleAltFirePressed()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandleAltFirePressed();
}

void AFPPlayerController::HandleAltFireReleased()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandleAltFireReleased();
}

void AFPPlayerController::HandleInteractPressed()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandleInteractPressed();
}

void AFPPlayerController::HandleInteractReleased()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandleInteractReleased();
}

void AFPPlayerController::HandleMeleeSelected()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandleMeleeSelected();
}

void AFPPlayerController::HandlePrimarySelected()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandlePrimarySelected();
}

void AFPPlayerController::HandleSecondarySelected()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandleSecondarySelected();
}

void AFPPlayerController::HandleTestCheckAndDropWeapon()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->CheckAndDropWeapon();
}

void AFPPlayerController::HandlePlayerPinged()
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);
	playerPawn->HandlePlayerPinged();
}

void AFPPlayerController::MoveForward(float input)
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);

	if (playerPawn != nullptr)
	{
		playerPawn->MoveForward(input);
	}
}

void AFPPlayerController::MoveRight(float input)
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);

	if (playerPawn != nullptr)
	{
		playerPawn->MoveRight(input);
	}
}

void AFPPlayerController::Turn(float input)
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);

	if (playerPawn != nullptr)
	{
		playerPawn->Turn(input);
	}
}

void AFPPlayerController::LookUp(float input)
{
	AActor* pawn = GetPawn();
	AFPPlayer* playerPawn = Cast<AFPPlayer>(pawn);

	if (playerPawn != nullptr)
	{
		playerPawn->LookUp(input);
	}
}