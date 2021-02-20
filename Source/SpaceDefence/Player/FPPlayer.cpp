// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayer.h"
#include "../Public/Projectile/TD_BaseProjectile.h"
#include "../Common/InteractionComponent.h"
#include "./Weapons/BaseWeapon.h"
#include "../Interactibles/InteractionDisplayManager.h"
#include "../Markers/WorldPingComponent.h"
#include "../Markers/WorldPingMarker.h"
#include "../Common/HealthAndDamageComp.h"
#include "../Public/TD_GameModeFPS.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFPPlayer::AFPPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBooom"));
	CameraLeftHandView = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLeftHandView"));
	CameraRightHandView = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRightHandView"));
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	GroundCheckPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GroundCheckPoint"));
	WallCheckPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WallCheckPoint"));
	InteractionCastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionCastPoint"));
	WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAttachPoint"));
	HealthAndDamage = CreateDefaultSubobject<UHealthAndDamageComp>(TEXT("HealthAndDamage"));

	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bUsePawnControlRotation = true;

	CameraLeftHandView->SetupAttachment(CharacterCamera);
	CameraRightHandView->SetupAttachment(CharacterCamera);
	CharacterCamera->SetupAttachment(CameraBoom);
	GroundCheckPoint->SetupAttachment(GetCapsuleComponent());
	WallCheckPoint->SetupAttachment(GetCapsuleComponent());
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

	WeaponAttachPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	WeaponAttachPoint->SetRelativeLocation(AttachRelativeLocation);
	WeaponAttachPoint->SetRelativeRotation(AttachRelativeRotation);

	OnPlayerLanded.AddDynamic(this, &AFPPlayer::PlayerLanded);

	_slideTimer = 0;
	_targetRecoilRotation = FVector::ZeroVector;

	PushPlayerMovementState(EPlayerMovementState::Walk);

	AActor* weapon = GetWorld()->SpawnActor(MeleeWeapon, &FVector::ZeroVector, &FRotator::ZeroRotator);
	ABaseWeapon* meleeWeapon = Cast<ABaseWeapon>(weapon);
	PickupMeleeWeapon(meleeWeapon);

	SetCapsuleData(DefaultHalfHeight, DefaultRadius);
	ApplyChangesToCharacter();

	_initialized = true;
}

void AFPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGroundStatus();
	UpdateCharacterSliding(DeltaTime);
	UpdateInteractibleCollection(DeltaTime);

	FireUpdate(DeltaTime);
	UpdateRecoilRotation(DeltaTime);
	WallClimbCheck(DeltaTime);
	UpdateCapsuleSize(DeltaTime);
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
			RemovePlayerMovementState(EPlayerMovementState::Slide);
			PushPlayerMovementState(EPlayerMovementState::Crouch);
			ApplyChangesToCharacter();
		}
	}
}

void AFPPlayer::WallClimbCheck(float deltaTime)
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

		float duration = playerController->GetInputKeyTimeDown(key); // TODO: Change this when custom input is added...
		if (duration != 0)
		{
			_currentClimbTime += deltaTime;
			if (_currentClimbTime <= ClimbDuration)
			{
				_isClimbing = true;
				LaunchCharacter(FVector(0, 0, ClimbVelocity), true, true);
			}
			else if (_isOnGround)
			{
				_isClimbing = false;
				_currentClimbTime = 0;
			}
		}
		else
		{
			_isClimbing = false;
			_currentClimbTime = 0;
		}
	}
	else
	{
		_isClimbing = false;
		_currentClimbTime = 0;
	}

	// So that when climbing the jump state is not pushed
	if (_isClimbing)
	{
		RemovePlayerMovementState(EPlayerMovementState::Jump);
		RemovePlayerMovementState(EPlayerMovementState::RunJump);
	}
}

bool AFPPlayer::IsClimbing()
{
	return _isClimbing;
}

void AFPPlayer::MoveForward(float value)
{
	if (!_initialized)
	{
		return;
	}

	_verticalInput = value;

	auto lastState = GetTopPlayerState();
	if (lastState != EPlayerMovementState::Slide)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

float AFPPlayer::GetVerticalInput()
{
	return _verticalInput;
}

void AFPPlayer::MoveRight(float value)
{
	if (!_initialized)
	{
		return;
	}

	_horizontalInput = value;

	if (_isClimbing)
	{
		return;
	}

	if (GetTopPlayerState() != EPlayerMovementState::Walk && GetTopPlayerState() != EPlayerMovementState::Crouch)
	{
		return;
	}

	if (value != 0)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

float AFPPlayer::GetHorizontalInput()
{
	return _horizontalInput;
}

void AFPPlayer::Turn(float value)
{
	if (!_initialized)
	{
		return;
	}

	if (_isClimbing)
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
	if (!_initialized)
	{
		return;
	}

	if (_isClimbing)
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
	if (!_isOnGround)
	{
		return;
	}

	RemovePlayerMovementState(EPlayerMovementState::Crouch);

	if (GetTopPlayerState() == EPlayerMovementState::Slide)
	{
		StopCharacterSliding();

		RemovePlayerMovementState(EPlayerMovementState::Slide);
		PushPlayerMovementState(EPlayerMovementState::RunJump);
		PlayerRunJumped();
	}
	else if (GetTopPlayerState() == EPlayerMovementState::Run)
	{
		PushPlayerMovementState(EPlayerMovementState::RunJump);
		PlayerRunJumped();
	}
	else
	{
		PushPlayerMovementState(EPlayerMovementState::Jump);
		PlayerJumped();
	}

	FTimerHandle unusedHandle;
	ApplyChangesToCharacter();
	GetWorldTimerManager().SetTimer(unusedHandle, this, &AFPPlayer::FrameDelayedJump, GetWorld()->GetDeltaSeconds(), false);
}

void AFPPlayer::FrameDelayedJump()
{
	FVector directionVector = GetActorForwardVector() * _verticalInput;
	FVector velocityDirection = GetVelocity();
	float velocityMag = velocityDirection.Size();

	directionVector.X *= JumpVelocity.X * velocityMag;
	directionVector.Y *= JumpVelocity.Y * velocityMag;
	directionVector.Z = JumpVelocity.Z;

	LaunchCharacter(directionVector, true, true);
}

void AFPPlayer::RunPressed()
{
	StopCharacterSliding();

	RemovePlayerMovementState(EPlayerMovementState::Crouch);
	PushPlayerMovementState(EPlayerMovementState::Run);

	ApplyChangesToCharacter();
	PlayerRunStarted();
}

void AFPPlayer::RunReleased()
{
	RemovePlayerMovementState(EPlayerMovementState::Run);
	ApplyChangesToCharacter();
	PlayerRunEnded();
}

bool AFPPlayer::IsRunning()
{
	EPlayerMovementState playerLastState = GetTopPlayerState();
	return playerLastState == EPlayerMovementState::Run && _verticalInput == 1 && !_isClimbing && _isOnGround;
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
	if (_currentInteractionComponent != nullptr && _currentInteractionComponent->IsInteractionActive())
	{
		auto actor = _currentInteractionComponent->GetOwner();
		float distance = FVector::Distance(actor->GetActorLocation(), GetActorLocation());
		if (distance > MaxInteractionDistance)
		{
			HandleInteractReleased();
			return;
		}

		bool interactionComplete = _currentInteractionComponent->InteractionUpdate(deltaTime);
		if (interactionComplete)
		{
			switch (_currentInteractionComponent->GetInteractibleType())
			{
			case EInteractibleType::Weapon:
				PickupWeapon(Cast<ABaseWeapon>(_currentInteractionComponent->GetOwner()));
				break;
			}

			ATD_GameModeFPS* gameMode = Cast<ATD_GameModeFPS>(GetWorld()->GetAuthGameMode());
			gameMode->InteractionDisplayManagerRef->HideInteractionBar();

			ClearInteractableObject();
		}
		else
		{
			ATD_GameModeFPS* gameMode = Cast<ATD_GameModeFPS>(GetWorld()->GetAuthGameMode());
			gameMode->InteractionDisplayManagerRef->SetInteractionBarProgress(_currentInteractionComponent->GetInteractionProgress());
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
			UActorComponent* actorComponent = tempActor->GetComponentByClass(UInteractionComponent::StaticClass());
			UInteractionComponent* interactionComponent = Cast<UInteractionComponent>(actorComponent);

			if (interactionComponent != nullptr)
			{
				_currentInteractionComponent = interactionComponent;

				ATD_GameModeFPS* gameMode = Cast<ATD_GameModeFPS>(GetWorld()->GetAuthGameMode());
				gameMode->InteractionDisplayManagerRef->ShowInteractionBar();
			}
		}
		else
		{
			ATD_GameModeFPS* gameMode = Cast<ATD_GameModeFPS>(GetWorld()->GetAuthGameMode());
			gameMode->InteractionDisplayManagerRef->HideInteractionBar();
		}
	}
}

void AFPPlayer::ClearInteractableObject()
{
	_currentInteractionComponent = nullptr;
}

void AFPPlayer::HandleInteractPressed()
{
	if (_currentInteractionComponent != nullptr)
	{
		_currentInteractionComponent->SetInteractionTime();
		_currentInteractionComponent->StartInteraction();
	}
}

void AFPPlayer::HandleInteractReleased()
{
	if (_currentInteractionComponent != nullptr)
	{
		ATD_GameModeFPS* gameMode = Cast<ATD_GameModeFPS>(GetWorld()->GetAuthGameMode());
		gameMode->InteractionDisplayManagerRef->SetInteractionBarProgress(0);
		_currentInteractionComponent->CancelInteraction();

		ClearInteractableObject();
	}
}

void AFPPlayer::UpdateGroundStatus()
{
	FHitResult hitResult;
	FVector startPosition = GroundCheckPoint->GetComponentLocation();
	FVector endPosition = startPosition + FVector::DownVector * GroundCheckDistance;

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility, collisionParams);
	if (!_isOnGround && hit)
	{
		OnPlayerLanded.Broadcast();
	}
	_isOnGround = hit;
}

bool AFPPlayer::IsOnGround()
{
	return _isOnGround;
}

bool AFPPlayer::IsFalling()
{
	FVector velocity = GetVelocity();
	float zVelocity = velocity.Z;

	if (zVelocity < -FallVelocityThreshold)
	{
		return true;
	}

	return false;
}

bool AFPPlayer::IsInFastMovementState()
{
	EPlayerMovementState movementState = GetTopPlayerState();

	switch (movementState)
	{
	case EPlayerMovementState::None:
	case EPlayerMovementState::Walk:
	case EPlayerMovementState::Crouch:
		return false;

	case EPlayerMovementState::Run:
	case EPlayerMovementState::Jump:
	case EPlayerMovementState::RunJump:
	case EPlayerMovementState::Slide:
		return true;

	default:
		return false;
	}
}

void AFPPlayer::PushPlayerMovementState(EPlayerMovementState movementState)
{
	if (_movementStack.Num() > 0 && GetTopPlayerState() == movementState)
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
	MovementStatePushed(GetTopPlayerState()); // This is just an event used to display the state being applied
	SetCapsuleData(DefaultHalfHeight, DefaultRadius);

	switch (GetTopPlayerState())
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
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		SetCapsuleData(CrouchHalfHeight, CrouchRadius);
	}
	break;

	case EPlayerMovementState::Slide:
	{
		GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;
		SetCapsuleData(CrouchHalfHeight, CrouchRadius);
	}
	break;

	default:
		break;
	}
}

EPlayerMovementState AFPPlayer::GetTopPlayerState()
{
	if (_movementStack.Num() <= 0)
	{
		return EPlayerMovementState::None;
	}

	return _movementStack.Last();
}

void AFPPlayer::SetCapsuleData(float targetHeight, float targetRadius)
{
	_capsuleHeight = FVector2D(targetHeight, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	_capsuleRadius = FVector2D(targetRadius, GetCapsuleComponent()->GetUnscaledCapsuleRadius());
	_capsuleLerpAmount = 0;
}

void AFPPlayer::UpdateCapsuleSize(float deltaTime)
{
	if (_capsuleLerpAmount > 1 || _capsuleLerpAmount < 0)
	{
		CameraBoom->bEnableCameraRotationLag = false;
		return;
	}

	CameraBoom->bEnableCameraRotationLag = true;
	float currentHeight = FMath::Lerp(_capsuleHeight.Y, _capsuleHeight.X, _capsuleLerpAmount);
	float currentRadius = FMath::Lerp(_capsuleRadius.Y, _capsuleRadius.X, _capsuleLerpAmount);

	GetCapsuleComponent()->SetCapsuleHalfHeight(currentHeight);
	GetCapsuleComponent()->SetCapsuleRadius(currentRadius);

	_capsuleLerpAmount += CapsuleSizeLerpRate * deltaTime;
	if (_capsuleLerpAmount > 1)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(_capsuleHeight.X);
		GetCapsuleComponent()->SetCapsuleRadius(_capsuleRadius.X);
	}
}

void AFPPlayer::StopCharacterSliding()
{
	FRotator cameraRotation = CameraBoom->GetRelativeRotation();
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);
	FRotator currentRotation = playerController->GetControlRotation();
	playerController->SetControlRotation(currentRotation + cameraRotation);
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

				_receoilDelay = true;
				SpawnWeaponProjectile(_primaryWeapon->GetProjectile(), _primaryWeapon->GetShootingPoint());

				FRecoilOffset recoilOffset = _primaryWeapon->GetCurrentRecoilData();
				if (IsInFastMovementState())
				{
					recoilOffset.offset *= _primaryWeapon->FastMovementRecoilMultiplier;
				}
				int maxRecoilCount = _primaryWeapon->GetMaxRecoilCount();

				FTimerDelegate timerHandle;
				FTimerHandle unusedHandle;
				timerHandle.BindUFunction(this, FName("DelayedCameraMovement"), _primaryWeapon, recoilOffset, maxRecoilCount);
				GetWorldTimerManager().SetTimer(unusedHandle, timerHandle, RECOIL_CAMERA_DELAY, false);
			}
		}
		break;

		case EPlayerWeapon::Secondary:
		{
			if (_secondaryWeapon->ShootTick(deltaTime))
			{
				_secondaryWeapon->Shoot();

				_receoilDelay = true;
				SpawnWeaponProjectile(_secondaryWeapon->GetProjectile(), _secondaryWeapon->GetShootingPoint());

				FRecoilOffset recoilOffset = _secondaryWeapon->GetCurrentRecoilData();
				if (IsInFastMovementState())
				{
					recoilOffset.offset *= _secondaryWeapon->FastMovementRecoilMultiplier;
				}
				int maxRecoilCount = _secondaryWeapon->GetMaxRecoilCount();

				FTimerDelegate timerHandle;
				FTimerHandle unusedHandle;
				timerHandle.BindUFunction(this, FName("DelayedCameraMovement"), _secondaryWeapon, recoilOffset, maxRecoilCount);
				GetWorldTimerManager().SetTimer(unusedHandle, timerHandle, RECOIL_CAMERA_DELAY, false);
			}
		}
		break;
		}
	}
}

void AFPPlayer::UpdateRecoilRotation(float deltaTime)
{
	if (_recoilLerpAmount > 1 || _recoilLerpAmount < 0 || _receoilDelay)
	{
		return;
	}

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FRotator currentRotation = playerController->GetControlRotation();

	float startLerp = _recoilLerpAmount;
	_recoilLerpAmount += RecoilRotationLerpRate * deltaTime;
	float endLerp = _recoilLerpAmount;
	if (endLerp > 1)
	{
		endLerp = 1;
	}
	float difference = endLerp - startLerp;

	FVector targetVector = _targetRecoilRotation * difference;
	FRotator targetRotation = currentRotation + FRotator(targetVector.Y, targetVector.X, 0);

	playerController->SetControlRotation(targetRotation);

	if (_recoilLerpAmount > 1)
	{
		_targetRecoilRotation = FVector::ZeroVector;
	}
}

void AFPPlayer::DelayedCameraMovement(ABaseWeapon* baseWeapon, FRecoilOffset recoilOffset, int maxRecoilCount)
{
	WeaponShotCameraShake(baseWeapon->CameraShake);
	SetRecoilCameraPosition(recoilOffset, maxRecoilCount);

	_receoilDelay = false;
}

void AFPPlayer::SetRecoilCameraPosition(FRecoilOffset recoilOffset, int maxRecoilCount)
{
	if (recoilOffset.index == maxRecoilCount - 1)
	{
		// Don't do anything...
	}
	else
	{
		FVector2D offset = recoilOffset.offset;
		_targetRecoilRotation = _targetRecoilRotation * (1 - _recoilLerpAmount) + FVector(offset.X, offset.Y, 0);
		_recoilLerpAmount = 0;
	}
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
	GetWorld()->SpawnActor<ATD_BaseProjectile>(projectile, spawnPoint, spawnRotation, spawnParams);
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
	{
		weapon->PickupWeapon();
		PickupPrimaryWeapon(weapon);
	}
	break;

	case EPlayerWeapon::Secondary:
	{
		weapon->PickupWeapon();
		PickupSecondaryWeapon(weapon);
	}
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
	_primaryWeapon->DropWeapon();

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
	_secondaryWeapon->DropWeapon();

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

void AFPPlayer::HandlePlayerPinged()
{
	FVector startingPoint = CharacterCamera->GetComponentLocation();
	FVector forwardVector = CharacterCamera->GetForwardVector();
	FVector endingPoint = startingPoint + forwardVector * MaxShootRayCastRange;

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startingPoint, endingPoint, ECollisionChannel::ECC_Visibility, collisionParams);
	if (hit && hitResult.GetActor() != nullptr)
	{
		auto hitActor = hitResult.GetActor();
		UActorComponent* actorComponent = hitActor->GetComponentByClass(UWorldPingComponent::StaticClass());

		FVector spawnPoint = hitResult.ImpactPoint + PingSpawnOffset;
		AActor* spawnedActorInstance = GetWorld()->SpawnActor(PingMarker, &spawnPoint, &FRotator::ZeroRotator);

		if (actorComponent != nullptr)
		{
			UWorldPingComponent* pingComponent = Cast<UWorldPingComponent>(actorComponent);
			UTexture2D* pingTexture = pingComponent->GetPingTexture();
			float pingTime = pingComponent->GetPingTime();

			AWorldPingMarker* worldPingMarker = Cast<AWorldPingMarker>(spawnedActorInstance);
			worldPingMarker->UpdateWidgetTexture(pingTexture);
			worldPingMarker->SetPingTime(pingTime);
		}
	}
}