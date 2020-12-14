// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPPlayer::AFPPlayer()
{
	FpMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBooom"));
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	GroundCheckPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GroundCheckPoint"));

	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bUsePawnControlRotation = true;

	CharacterCamera->SetupAttachment(CameraBoom);
	FpMesh->SetupAttachment(CharacterCamera);
	GroundCheckPoint->SetupAttachment(GetCapsuleComponent());

	HSensitivityMultiplier = 1;
	VSensitivityMultiplier = 1;
	GroundCheckDistance = 30;
	PrimaryActorTick.bCanEverTick = true;
}

void AFPPlayer::BeginPlay()
{
	SetPlayerMovementState(PlayerMovementState::Walk);
	SetDelayedMovementState(PlayerMovementState::None);
	ResetMovementVariables();

	OnPlayerLanded.AddDynamic(this, &AFPPlayer::PlayerLanded);
	Super::BeginPlay();
}

void AFPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGroundStatus();
}

void AFPPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFPPlayer::CharacterJump);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AFPPlayer::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AFPPlayer::RunReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPPlayer::LookUp);
}

void AFPPlayer::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void AFPPlayer::MoveRight(float value)
{
	if (_playerMovementState != PlayerMovementState::Walk && _playerMovementState != PlayerMovementState::Crouch)
	{
		return;
	}

	if (value != 0)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AFPPlayer::Turn(float value)
{
	AddControllerYawInput(value * TurnSpeed * HSensitivityMultiplier * GetWorld()->GetDeltaSeconds());
}

void AFPPlayer::LookUp(float value)
{
	AddControllerPitchInput(value * LookUpRate * VSensitivityMultiplier * GetWorld()->GetDeltaSeconds());
}

void AFPPlayer::CharacterJump()
{
	Jump();

	if (_playerMovementState == PlayerMovementState::Run)
	{
		SetPlayerMovementState(PlayerMovementState::RunJump);
	}
	else
	{
		SetPlayerMovementState(PlayerMovementState::Jump);
	}
}

void AFPPlayer::RunPressed()
{
	if (!_isOnGround)
	{
		SetDelayedMovementState(PlayerMovementState::Run);
	}
	else
	{
		SetPlayerMovementState(PlayerMovementState::Run);
	}

	ResetMovementVariables();
}

void AFPPlayer::RunReleased()
{
	if (!_isOnGround && _delayedMovementState == PlayerMovementState::Run)
	{
		SetDelayedMovementState(PlayerMovementState::None);
	}
	else if (_isOnGround)
	{
		SetPlayerMovementState(PlayerMovementState::Walk);
	}

	ResetMovementVariables();
}

void AFPPlayer::SetPlayerMovementState(PlayerMovementState movementState)
{
	if (_playerMovementState == movementState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Movement: Previous State And Last State Is Same");
	}

	_playerMovementState = movementState;
}

void AFPPlayer::SetDelayedMovementState(PlayerMovementState delayedState)
{
	_delayedMovementState = delayedState;
}

void AFPPlayer::UpdateGroundStatus()
{
	FHitResult hitResult;
	FVector startPosition = GroundCheckPoint->GetComponentLocation();
	FVector endPosition = startPosition + FVector::DownVector * GroundCheckDistance;

	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility);
	if (!_isOnGround && hit)
	{
		OnPlayerLanded.Broadcast();
	}
	_isOnGround = hit;
}

void AFPPlayer::PlayerLanded()
{
	if (_delayedMovementState != PlayerMovementState::None)
	{
		SetPlayerMovementState(_delayedMovementState);
		SetDelayedMovementState(PlayerMovementState::None);
	}

	ResetMovementVariables();
}

void AFPPlayer::ResetMovementVariables()
{
	switch (_playerMovementState)
	{
	case PlayerMovementState::None:
		break;

	case PlayerMovementState::Walk:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;

	case PlayerMovementState::Run:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;

	case PlayerMovementState::Jump:
		break;

	case PlayerMovementState::RunJump:
		break;

	case PlayerMovementState::Crouch:
		break;

	default:
		break;
	}
}