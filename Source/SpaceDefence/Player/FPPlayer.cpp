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
	_slideTimer = 0;

	PushPlayerMovementState(EPlayerMovementState::Walk);
	ApplyChangesToCharacter();

	OnPlayerLanded.AddDynamic(this, &AFPPlayer::PlayerLanded);
	Super::BeginPlay();
}

void AFPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGroundStatus();
	UpdateCharacterSliding(DeltaTime);
}

void AFPPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFPPlayer::CharacterJump);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AFPPlayer::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AFPPlayer::RunReleased);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AFPPlayer::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AFPPlayer::CrouchReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPPlayer::LookUp);
}

void AFPPlayer::UpdateCharacterSliding(float deltaTime)
{
	if (_slideTimer > 0)
	{
		_slideTimer -= deltaTime;
		if (_slideTimer <= 0)
		{
			RemovePlayerMovementState(EPlayerMovementState::Slide);
			ApplyChangesToCharacter();
		}
		else
		{
			AddMovementInput(GetActorForwardVector(), 1);
		}
	}
}

void AFPPlayer::MoveForward(float value)
{
	if (_movementStack.Last() == EPlayerMovementState::Walk || _movementStack.Last() == EPlayerMovementState::Run ||
		_movementStack.Last() == EPlayerMovementState::Crouch)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFPPlayer::MoveRight(float value)
{
	if (_movementStack.Last() != EPlayerMovementState::Walk && _movementStack.Last() != EPlayerMovementState::Crouch)
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

	if (_movementStack.Last() == EPlayerMovementState::Run || _movementStack.Last() == EPlayerMovementState::Slide)
	{
		RemovePlayerMovementState(EPlayerMovementState::Slide);
		PushPlayerMovementState(EPlayerMovementState::RunJump);
	}
	else
	{
		PushPlayerMovementState(EPlayerMovementState::Jump);
	}
}

void AFPPlayer::RunPressed()
{
	PushPlayerMovementState(EPlayerMovementState::Run);
	ApplyChangesToCharacter();
}

void AFPPlayer::RunReleased()
{
	RemovePlayerMovementState(EPlayerMovementState::Run);
	ApplyChangesToCharacter();
}

void AFPPlayer::CrouchPressed()
{
	if (HasPlayerState(EPlayerMovementState::Run) || HasPlayerState(EPlayerMovementState::Jump))
	{
		PushPlayerMovementState(EPlayerMovementState::Slide);
		_slideTimer = SlideDuration;
	}
	else
	{
		PushPlayerMovementState(EPlayerMovementState::Crouch);
	}

	ApplyChangesToCharacter();
}

void AFPPlayer::CrouchReleased()
{
	RemovePlayerMovementState(EPlayerMovementState::Crouch);
	ApplyChangesToCharacter();
}

void AFPPlayer::PlayerLanded()
{
	RemovePlayerMovementState(EPlayerMovementState::Jump);
	RemovePlayerMovementState(EPlayerMovementState::RunJump);

	ApplyChangesToCharacter();
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

void AFPPlayer::PushPlayerMovementState(EPlayerMovementState movementState)
{
	if (_movementStack.Num() > 0 && _movementStack.Last() == movementState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Movement: Previous State And Last State Is Same");
	}

	MovementStatePushed(movementState);
	_movementStack.Push(movementState);
}

void AFPPlayer::RemovePlayerMovementState(EPlayerMovementState movementState)
{
	for (int i = _movementStack.Num() - 1; i >= 0; i--)
	{
		if (_movementStack[i] == movementState)
		{
			_movementStack.RemoveAt(i);
		}
	}
}

bool AFPPlayer::HasPlayerState(EPlayerMovementState movementState)
{
	for (int i = 0; i < _movementStack.Num(); i++)
	{
		if (_movementStack[i] == movementState)
		{
			return true;
		}
	}

	return false;
}


void AFPPlayer::ApplyChangesToCharacter()
{
	switch (_movementStack.Last())
	{
	case EPlayerMovementState::None:
		break;

	case EPlayerMovementState::Walk:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;

	case EPlayerMovementState::Run:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;

	case EPlayerMovementState::Jump:
		break;

	case EPlayerMovementState::RunJump:
		break;

	case EPlayerMovementState::Crouch:
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		break;

	case EPlayerMovementState::Slide:
		GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;
		break;

	default:
		break;
	}
}