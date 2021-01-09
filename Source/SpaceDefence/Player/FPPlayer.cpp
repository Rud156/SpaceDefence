// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayer.h"
#include "./Projectiles/BasePlayerProjectile.h"
#include "../Interactibles/IntfBaseInteractible.h"
#include "./Weapons/BaseWeapon.h"
#include "../Interactibles/InteractionDisplayManager.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFPPlayer::AFPPlayer()
{
	FpMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBooom"));
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	GroundCheckPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GroundCheckPoint"));
	WallCheckPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WallCheckPoint"));
	WeaponTempShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponTempShootingPoint"));
	InteractionCastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionCastPoint"));
	WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAttachPoint"));

	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bUsePawnControlRotation = true;

	CharacterCamera->SetupAttachment(CameraBoom);
	FpMesh->SetupAttachment(CharacterCamera);
	GroundCheckPoint->SetupAttachment(GetCapsuleComponent());
	WallCheckPoint->SetupAttachment(GetCapsuleComponent());
	WeaponTempShootingPoint->SetupAttachment(CharacterCamera);
	InteractionCastPoint->SetupAttachment(CharacterCamera);
	WeaponAttachPoint->SetupAttachment(CharacterCamera);

	HSensitivityMultiplier = 1;
	VSensitivityMultiplier = 1;
	GroundCheckDistance = 30;
	PrimaryActorTick.bCanEverTick = true;
}

void AFPPlayer::BeginPlay()
{
	Super::BeginPlay();

	OnPlayerLanded.AddDynamic(this, &AFPPlayer::PlayerLanded);
	auto interactionActor = UGameplayStatics::GetActorOfClass(GetWorld(), AInteractionDisplayManager::StaticClass());
	AInteractionDisplayManager* interactionDisplayInstance = Cast<AInteractionDisplayManager>(interactionActor);
	if (interactionDisplayInstance != nullptr)
	{
		_interactionManager = interactionDisplayInstance;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Interaction Display Not Found!!!");
	}

	_slideTimer = 0;

	PushPlayerMovementState(EPlayerMovementState::Walk);
	ApplyChangesToCharacter();

	AActor* weapon = GetWorld()->SpawnActor(MeleeWeapon, &FVector::ZeroVector, &FRotator::ZeroRotator);
	ABaseWeapon* meleeWeapon = Cast<ABaseWeapon>(weapon);
	PickupMeleeWeapon(meleeWeapon);
}

void AFPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGroundStatus();
	UpdateCharacterSliding(DeltaTime);
	UpdateInteractibleCollection(DeltaTime);

	FireUpdate(DeltaTime);
	WallClimbCheck();
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
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AFPPlayer::FirePressed);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AFPPlayer::FireReleased);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AFPPlayer::HandleInteractPressed);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &AFPPlayer::HandleInteractReleased);
	PlayerInputComponent->BindAction("Melee", EInputEvent::IE_Pressed, this, &AFPPlayer::HandleMeleeSelected);
	PlayerInputComponent->BindAction("Primary", EInputEvent::IE_Pressed, this, &AFPPlayer::HandlePrimarySelected);
	PlayerInputComponent->BindAction("Secondary", EInputEvent::IE_Pressed, this, &AFPPlayer::HandleSecondarySelected);
	PlayerInputComponent->BindAction("TestDropCurrentWeapon", IE_Pressed, this, &AFPPlayer::CheckAndDropWeapon);

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
			StopCharacterSliding();
			RemovePlayerMovementState(EPlayerMovementState::Slide);
			PushPlayerMovementState(EPlayerMovementState::Crouch);
			ApplyChangesToCharacter();
		}
		else
		{
			AddMovementInput(GetActorForwardVector(), 1);
		}

		auto velocity = GetVelocity().Size();
		if (velocity < MinSlideSpeed)
		{
			StopCharacterSliding();
			PushPlayerMovementState(EPlayerMovementState::Crouch);
			ApplyChangesToCharacter();
		}
	}
}

void AFPPlayer::WallClimbCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	FVector startPosition = GetActorLocation();
	FVector endPosition = WallCheckPoint->GetComponentLocation();

	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility, collisionParams);
	if (hit && hitResult.GetActor() != nullptr && !hitResult.GetActor()->ActorHasTag(NotClimbableTag))
	{
		auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FKey key = EKeys::SpaceBar;

		float duration = playerController->GetInputKeyTimeDown(key);
		_currentClimbTime = duration;

		if (_currentClimbTime != 0)
		{
			if (_currentClimbTime <= ClimbDuration)
			{
				LaunchCharacter(FVector(0, 0, ClimbVelocity), true, true);
			}
		}
	}
	else
	{
		_currentClimbTime = 0;
	}
}

void AFPPlayer::MoveForward(float value)
{
	if (_currentClimbTime != 0)
	{
		return;
	}

	auto lastState = _movementStack.Last();
	if (lastState != EPlayerMovementState::Slide)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFPPlayer::MoveRight(float value)
{
	if (_currentClimbTime != 0)
	{
		return;
	}

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
	if (_currentClimbTime != 0)
	{
		return;
	}

	float turnAmount = value * TurnSpeed * HSensitivityMultiplier * GetWorld()->GetDeltaSeconds();
	if (HasPlayerState(EPlayerMovementState::Slide))
	{
		CameraBoom->AddRelativeRotation(FRotator(0, turnAmount, 0));
	}
	else
	{
		AddControllerYawInput(turnAmount);
	}
}

void AFPPlayer::LookUp(float value)
{
	if (_currentClimbTime != 0)
	{
		return;
	}

	float turnAmount = value * LookUpRate * VSensitivityMultiplier * GetWorld()->GetDeltaSeconds();
	if (HasPlayerState(EPlayerMovementState::Slide))
	{
		CameraBoom->AddRelativeRotation(FRotator(-turnAmount, 0, 0));
	}
	else
	{
		AddControllerPitchInput(turnAmount);
	}
}

void AFPPlayer::CharacterJump()
{
	RemovePlayerMovementState(EPlayerMovementState::Crouch);

	if (_movementStack.Last() == EPlayerMovementState::Slide)
	{
		StopCharacterSliding();

		RemovePlayerMovementState(EPlayerMovementState::Slide);
		PushPlayerMovementState(EPlayerMovementState::RunJump);
	}
	else if (_movementStack.Last() == EPlayerMovementState::Run)
	{
		PushPlayerMovementState(EPlayerMovementState::RunJump);
	}
	else
	{
		PushPlayerMovementState(EPlayerMovementState::Jump);
	}

	FTimerHandle unusedHandle;
	ApplyChangesToCharacter();
	GetWorldTimerManager().SetTimer(unusedHandle, this, &AFPPlayer::FrameDelayedJump, GetWorld()->GetDeltaSeconds(), false);
}

void AFPPlayer::FrameDelayedJump()
{
	FVector directionVector = GetActorForwardVector();
	float velocity = GetVelocity().Size();

	directionVector.X *= JumpVelocity.X * velocity;
	directionVector.Y *= JumpVelocity.Y * velocity;
	directionVector.Z = JumpVelocity.Z;

	LaunchCharacter(directionVector, false, false);
}

void AFPPlayer::RunPressed()
{
	StopCharacterSliding();

	RemovePlayerMovementState(EPlayerMovementState::Crouch);
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
	StopCharacterSliding();

	if (HasPlayerState(EPlayerMovementState::Run) || HasPlayerState(EPlayerMovementState::Jump))
	{
		PushPlayerMovementState(EPlayerMovementState::Slide);
		_slideTimer = SlideDuration;
		CameraBoom->bUsePawnControlRotation = false;
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

void AFPPlayer::UpdateInteractibleCollection(float deltaTime)
{
	if (_currentInteractable != nullptr && _currentInteractable->InteractionStarted_Implementation())
	{
		auto actor = Cast<AActor>(_currentInteractable);
		float distance = FVector::Distance(actor->GetActorLocation(), GetActorLocation());
		if (distance > MaxInteractionDistance)
		{
			HandleInteractReleased();
			return;
		}

		bool interactionComplete = _currentInteractable->InteractUpdate_Implementation(deltaTime);
		if (interactionComplete)
		{
			switch (_currentInteractable->GetInteractibleType_Implementation())
			{
			case EInteractibleType::Weapon:
				PickupWeapon(Cast<ABaseWeapon>(_currentInteractable));
				break;
			}

			_interactionManager->HideInteractionBar();
			ClearInteractableObject();
		}
		else
		{
			_interactionManager->SetInteractionBarProgress(_currentInteractable->GetInteractionProgress_Implementation());
		}
	}
	else
	{
		FHitResult hitResult;
		FVector startPosition = InteractionCastPoint->GetComponentLocation();
		FVector endPosition = startPosition + CharacterCamera->GetForwardVector() * MaxInteractionDistance;

		bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility);
		if (hit && hitResult.GetActor() != nullptr)
		{
			AActor* tempActor = hitResult.GetActor();

			_currentInteractable = Cast<IIntfBaseInteractible>(tempActor);
			if (_currentInteractable != nullptr)
			{
				_interactionManager->ShowInteractionBar();
			}
		}
		else
		{
			_interactionManager->HideInteractionBar();
		}
	}
}

void AFPPlayer::SetInteractableObject(IIntfBaseInteractible* callingObject)
{
	_currentInteractable = callingObject;
}

void AFPPlayer::ClearInteractableObject()
{
	_currentInteractable = nullptr;
}

void AFPPlayer::HandleInteractPressed()
{
	if (_currentInteractable != nullptr)
	{
		_currentInteractable->SetInteractionTime_Implementation(1);
	}
}

void AFPPlayer::HandleInteractReleased()
{
	if (_currentInteractable != nullptr)
	{
		_interactionManager->SetInteractionBarProgress(0);
		_currentInteractable->CancelInteraction_Implementation();

		ClearInteractableObject();
	}
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
	MovementStatePushed(_movementStack.Last()); // This is just an event used to display the state being applied

	GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultHalfSize);
	GetCapsuleComponent()->SetCapsuleRadius(DefaultRadius);

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
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHalfHeight);
		GetCapsuleComponent()->SetCapsuleRadius(CrouchRadius);
		break;

	case EPlayerMovementState::Slide:
		GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHalfHeight);
		GetCapsuleComponent()->SetCapsuleRadius(CrouchRadius);
		break;

	default:
		break;
	}
}

void AFPPlayer::StopCharacterSliding()
{
	FRotator cameraRotation = CameraBoom->GetRelativeRotation();
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AddControllerPitchInput(cameraRotation.Pitch / playerController->InputPitchScale);
	AddControllerYawInput(cameraRotation.Yaw / playerController->InputYawScale);

	CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);
	CameraBoom->bUsePawnControlRotation = true;
	_slideTimer = 0;

	RemovePlayerMovementState(EPlayerMovementState::Slide);
	ApplyChangesToCharacter();
}

void AFPPlayer::FirePressed()
{
	_firePressed = true;
}

void AFPPlayer::FireReleased()
{
	_firePressed = false;
}

void AFPPlayer::FireUpdate(float deltaTime)
{
	if (_firePressed)
	{
		switch (_currentWeapon)
		{
		case EPlayerWeapon::Melee:
		{
			if (_meleeWeapon->ShootTick(deltaTime))
			{
				_meleeWeapon->Shoot();
			}
		}
		break;

		case EPlayerWeapon::Primary:
		{
			if (_primaryWeapon->ShootTick(deltaTime))
			{
				_primaryWeapon->Shoot();
				FVector2D recoilOffset = _primaryWeapon->GetCurrentRecoilOffset();
				UpdateRecoilCamera(recoilOffset);
				SpawnWeaponProjectile(_primaryWeapon->GetProjectile(), _primaryWeapon->GetShootingPoint());
			}
		}
		break;

		case EPlayerWeapon::Secondary:
		{
			if (_secondaryWeapon->ShootTick(deltaTime))
			{
				_secondaryWeapon->Shoot();
				FVector2D recoilOffset = _secondaryWeapon->GetCurrentRecoilOffset();
				UpdateRecoilCamera(recoilOffset);
				SpawnWeaponProjectile(_secondaryWeapon->GetProjectile(), _secondaryWeapon->GetShootingPoint());
			}
		}
		break;
		}
	}
}

void AFPPlayer::UpdateRecoilCamera(FVector2D recoilOffset)
{
	AddControllerYawInput(recoilOffset.X);
	AddControllerPitchInput(-recoilOffset.Y);
}

void AFPPlayer::SpawnWeaponProjectile(TSubclassOf<class AActor> projectile, FVector spawnPoint)
{
	FVector startingPoint = CharacterCamera->GetComponentLocation();
	FVector forwardVector = CharacterCamera->GetForwardVector();
	FVector endingPoint = startingPoint + forwardVector * MaxShootRayCastRange;

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	FVector targetPoint = endingPoint;

	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startingPoint, endingPoint, ECollisionChannel::ECC_Visibility, collisionParams);
	if (hit && hitResult.GetActor() != nullptr)
	{
		targetPoint = hitResult.ImpactPoint;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FRotator spawnRotation = UKismetMathLibrary::FindLookAtRotation(spawnPoint, targetPoint);
	GetWorld()->SpawnActor<ABasePlayerProjectile>(projectile, spawnPoint, spawnRotation, spawnParams);
}

EPlayerWeapon AFPPlayer::GetCurrentWeapon()
{
	return _currentWeapon;
}

void AFPPlayer::ChangeCurrentWeapon(EPlayerWeapon weapon)
{
	if (_currentWeapon == weapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Trying to Equip Same Weapon");
	}

	_currentWeapon = weapon;
	ApplyWeaponChangesToCharacter();
}

void AFPPlayer::PickupWeapon(ABaseWeapon* weapon)
{
	if (weapon == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Trying to Pickup NULL Weapon");
		return;
	}

	EPlayerWeapon weaponType = weapon->WeaponType;
	switch (weaponType)
	{
	case EPlayerWeapon::Melee:
		// Do nothing as Melee Weapon cannot be changed...
		break;

	case EPlayerWeapon::Primary:
		PickupPrimaryWeapon(weapon);
		break;

	case EPlayerWeapon::Secondary:
		PickupSecondaryWeapon(weapon);
		break;
	}
}

void AFPPlayer::CheckAndDropWeapon()
{
	switch (_currentWeapon)
	{
	case EPlayerWeapon::Melee:
		// Do nothing as Melee Weapon cannot be changed...
		break;

	case EPlayerWeapon::Primary:
		DropPrimaryWeapon();
		break;

	case EPlayerWeapon::Secondary:
		DropSecondaryWeapon();
		break;
	}
}


ABaseWeapon* AFPPlayer::GetPrimaryWeapon()
{
	return _primaryWeapon;
}

void AFPPlayer::PickupPrimaryWeapon(ABaseWeapon* primaryWeapon)
{
	if (_primaryWeapon != nullptr)
	{
		DropPrimaryWeapon();
	}

	_primaryWeapon = primaryWeapon;
	ChangeCurrentWeapon(EPlayerWeapon::Primary);

	FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		false);
	_primaryWeapon->AttachToComponent(WeaponAttachPoint, attachmentRules);
}

ABaseWeapon* AFPPlayer::DropPrimaryWeapon()
{
	FDetachmentTransformRules detachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true);
	_primaryWeapon->DetachFromActor(detachRules);

	auto weaponCopy = _primaryWeapon;
	_primaryWeapon = nullptr;

	if (HasSecondaryWeapon())
	{
		ChangeCurrentWeapon(EPlayerWeapon::Secondary);
	}
	else
	{
		ChangeCurrentWeapon(EPlayerWeapon::Melee);
	}

	return weaponCopy;
}

bool AFPPlayer::HasPrimaryWeapon()
{
	return _primaryWeapon != nullptr;
}

ABaseWeapon* AFPPlayer::GetSecondaryWeapon()
{
	return _secondaryWeapon;
}

void AFPPlayer::PickupSecondaryWeapon(ABaseWeapon* secondaryWeapon)
{
	if (_secondaryWeapon != nullptr)
	{
		DropSecondaryWeapon();
	}

	_secondaryWeapon = secondaryWeapon;
	ChangeCurrentWeapon(EPlayerWeapon::Secondary);

	FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		true);
	_secondaryWeapon->AttachToComponent(WeaponAttachPoint, attachmentRules);
}

ABaseWeapon* AFPPlayer::DropSecondaryWeapon()
{
	FDetachmentTransformRules detachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true);
	_secondaryWeapon->DetachFromActor(detachRules);

	auto weaponCopy = _secondaryWeapon;
	_secondaryWeapon = nullptr;

	if (HasPrimaryWeapon())
	{
		ChangeCurrentWeapon(EPlayerWeapon::Primary);
	}
	else
	{
		ChangeCurrentWeapon(EPlayerWeapon::Melee);
	}

	return weaponCopy;
}

bool AFPPlayer::HasSecondaryWeapon()
{
	return  _secondaryWeapon != nullptr;
}

ABaseWeapon* AFPPlayer::GetMeleeWeapon()
{
	return  _meleeWeapon;
}

bool AFPPlayer::HasMeleeWeapon()
{
	return _meleeWeapon != nullptr;
}

void AFPPlayer::PickupMeleeWeapon(ABaseWeapon* meleeWeapon)
{
	if (_meleeWeapon != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Trying to Pickup Second Melee Weapon!!!");
		return;
	}

	_meleeWeapon = meleeWeapon;
	ChangeCurrentWeapon(EPlayerWeapon::Melee);

	FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		true);
	_meleeWeapon->AttachToComponent(WeaponAttachPoint, attachmentRules);
}

void AFPPlayer::ApplyWeaponChangesToCharacter()
{
	if (_primaryWeapon != nullptr)
	{
		_primaryWeapon->HideWeapon();
	}
	if (_secondaryWeapon != nullptr)
	{
		_secondaryWeapon->HideWeapon();
	}
	if (_meleeWeapon != nullptr)
	{
		_meleeWeapon->HideWeapon();
	}

	switch (_currentWeapon)
	{
	case EPlayerWeapon::Melee:
		_meleeWeapon->ShowWeapon();
		break;

	case EPlayerWeapon::Primary:
		_primaryWeapon->ShowWeapon();
		break;

	case EPlayerWeapon::Secondary:
		_secondaryWeapon->ShowWeapon();
		break;

	default:
		// This should never be triggered...
		break;
	}
}

void AFPPlayer::HandleMeleeSelected()
{
	ChangeCurrentWeapon(EPlayerWeapon::Melee);
	ApplyWeaponChangesToCharacter();
}

void AFPPlayer::HandlePrimarySelected()
{
	if (HasPrimaryWeapon())
	{
		ChangeCurrentWeapon(EPlayerWeapon::Primary);
		ApplyWeaponChangesToCharacter();
	}
}

void AFPPlayer::HandleSecondarySelected()
{
	if (HasSecondaryWeapon())
	{
		ChangeCurrentWeapon(EPlayerWeapon::Secondary);
		ApplyWeaponChangesToCharacter();
	}
}