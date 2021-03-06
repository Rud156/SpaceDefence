// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayer.h"
#include "../Public/Projectile/TD_BaseProjectile.h"
#include "../Common/InteractionComponent.h"
#include "./Weapons/BaseWeapon.h"
#include "../Interactibles/InteractionDisplayManager.h"
#include "../Markers/WorldPingComponent.h"
#include "../Markers/WorldPingMarker.h"
#include "../Common/HealthAndDamageComp.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFPPlayer::AFPPlayer(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBooom"));
	CameraLeftHandView = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLeftHandView"));
	CameraRightHandView = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRightHandView"));
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	WallCheckPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WallCheckPoint"));
	InteractionCastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionCastPoint"));
	WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAttachPoint"));
	HealthAndDamage = CreateDefaultSubobject<UHealthAndDamageComp>(TEXT("HealthAndDamage"));

	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->bUsePawnControlRotation = true;

	CameraLeftHandView->SetupAttachment(CharacterCamera);
	CameraRightHandView->SetupAttachment(CharacterCamera);
	CharacterCamera->SetupAttachment(CameraBoom);
	WallCheckPoint->SetupAttachment(GetCapsuleComponent());
	InteractionCastPoint->SetupAttachment(CharacterCamera);
	WeaponAttachPoint->SetupAttachment(CharacterCamera);

	HSensitivityMultiplier = 1;
	VSensitivityMultiplier = 1;
	PrimaryActorTick.bCanEverTick = true;
}

void AFPPlayer::BeginPlay()
{
	Super::BeginPlay();

	OnPlayerLanded.AddDynamic(this, &AFPPlayer::PlayerLanded);

	_slideTimer = 0;
	_adsLerpAmount = 1;
	_targetRecoilRotation = FVector::ZeroVector;

	PushPlayerMovementState(EPlayerMovementState::Walk);

	AActor* weapon = GetWorld()->SpawnActor(MeleeWeapon, &FVector::ZeroVector, &FRotator::ZeroRotator);
	ABaseWeapon* meleeWeapon = Cast<ABaseWeapon>(weapon);
	PickupMeleeWeapon(meleeWeapon);

	SetCapsuleData(DefaultHalfHeight, DefaultRadius);
	ApplyChangesToCharacter();
}

void AFPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGroundStatus();
	UpdateCharacterSliding(DeltaTime);
	UpdateInteractibleCollection(DeltaTime);

	FireUpdate(DeltaTime);
	UpdateRecoilRotation(DeltaTime);
	UpdateADSWeaponPoint(DeltaTime);
	WallClimbCheck(DeltaTime);
	UpdateCapsuleSize(DeltaTime);

	UpdateLeftRightHandPosition();
}

void AFPPlayer::Client_MoveForward(const float Value)
{
	Server_MoveForward(Value);
	MoveForward(Value);
}

void AFPPlayer::Server_MoveForward_Implementation(const float Value)
{
	MoveForward(Value);
}

void AFPPlayer::MoveForward(const float Value)
{
	_verticalInput = Value;

	const auto lastState = GetTopPlayerState();
	if (lastState != EPlayerMovementState::Slide)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

float AFPPlayer::GetVerticalInput()
{
	return _verticalInput;
}

void AFPPlayer::Client_MoveRight(const float Value)
{
	Server_MoveRight(Value);
	MoveRight(Value);
}

void AFPPlayer::Server_MoveRight_Implementation(const float Value)
{
	MoveRight(Value);
}

void AFPPlayer::MoveRight(const float Value)
{
	_horizontalInput = Value;

	if (_isClimbing)
	{
		return;
	}

	if (GetTopPlayerState() != EPlayerMovementState::Walk && GetTopPlayerState() != EPlayerMovementState::Crouch)
	{
		return;
	}

	if (Value != 0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

float AFPPlayer::GetHorizontalInput()
{
	return _horizontalInput;
}

void AFPPlayer::Client_Turn(const float Value)
{
	Turn(Value);
}

void AFPPlayer::Server_Turn_Implementation(const float Value)
{
	Turn(Value);
}

void AFPPlayer::Turn(const float Value)
{
	if (_isClimbing)
	{
		return;
	}

	const float turnAmount = Value * TurnSpeed * HSensitivityMultiplier * GetWorld()->GetDeltaSeconds();
	if (HasPlayerState(EPlayerMovementState::Slide))
	{
		CameraBoom->AddRelativeRotation(FRotator(0, turnAmount, 0));
	}
	else
	{
		AddControllerYawInput(turnAmount);
	}
}

void AFPPlayer::Client_LookUp(const float Value)
{
	LookUp(Value);
}

void AFPPlayer::Server_LookUp_Implementation(const float Value)
{
	LookUp(Value);
}

void AFPPlayer::LookUp(const float Value)
{
	if (_isClimbing)
	{
		return;
	}

	const float turnAmount = Value * LookUpRate * VSensitivityMultiplier * GetWorld()->GetDeltaSeconds();
	if (HasPlayerState(EPlayerMovementState::Slide))
	{
		CameraBoom->AddRelativeRotation(FRotator(-turnAmount, 0, 0));
	}
	else
	{
		AddControllerPitchInput(turnAmount);
	}
}

void AFPPlayer::Client_HandleJumpPressed()
{
	Server_HandleJumpPressed();
	HandleJumpPressed();
}

void AFPPlayer::Server_HandleJumpPressed_Implementation()
{
	HandleJumpPressed();
}

void AFPPlayer::HandleJumpPressed()
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
	FVector directionVector = GetActorForwardVector() * _verticalInput + GetActorRightVector() * _horizontalInput;

	directionVector.X *= JumpVelocity.X;
	directionVector.Y *= JumpVelocity.Y;
	directionVector.Z = JumpVelocity.Z;

	LaunchCharacter(directionVector, true, true);
}

void AFPPlayer::PlayerLanded()
{
	RemovePlayerMovementState(EPlayerMovementState::Jump);
	RemovePlayerMovementState(EPlayerMovementState::RunJump);

	ApplyChangesToCharacter();
}

void AFPPlayer::UpdateGroundStatus()
{
	const bool isOnGround = !GetCharacterMovement()->IsFalling();

	if (!_isOnGround && isOnGround)
	{
		OnPlayerLanded.Broadcast();
	}
	_isOnGround = isOnGround;
}

bool AFPPlayer::IsOnGround()
{
	return _isOnGround;
}

bool AFPPlayer::IsFalling()
{
	const FVector velocity = GetVelocity();
	const float zVelocity = velocity.Z;

	if (zVelocity < -FallVelocityThreshold)
	{
		return true;
	}

	return false;
}

void AFPPlayer::Client_HandleRunPressed()
{
	Server_HandleRunPressed();
	HandleRunPressed();
}

void AFPPlayer::Server_HandleRunPressed_Implementation()
{
	HandleRunPressed();
}

void AFPPlayer::HandleRunPressed()
{
	StopCharacterSliding();

	RemovePlayerMovementState(EPlayerMovementState::Crouch);
	PushPlayerMovementState(EPlayerMovementState::Run);

	ApplyChangesToCharacter();
	PlayerRunStarted();
}

void AFPPlayer::Client_HandleRunReleased()
{
	Server_HandleRunReleased();
	HandleRunReleased();
}

void AFPPlayer::Server_HandleRunReleased_Implementation()
{
	HandleRunReleased();
}

void AFPPlayer::HandleRunReleased()
{
	RemovePlayerMovementState(EPlayerMovementState::Run);
	ApplyChangesToCharacter();
	PlayerRunEnded();
}

bool AFPPlayer::IsRunning()
{
	const EPlayerMovementState playerLastState = GetTopPlayerState();
	return playerLastState == EPlayerMovementState::Run && _verticalInput == 1 && !_isClimbing && _isOnGround;
}

void AFPPlayer::Client_HandleCrouchPressed()
{
	Server_HandleCrouchPressed();
	HandleCrouchPressed();
}

void AFPPlayer::Server_HandleCrouchPressed_Implementation()
{
	HandleCrouchPressed();
}

void AFPPlayer::HandleCrouchPressed()
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

void AFPPlayer::Client_HandleCrouchReleased()
{
	Server_HandleCrouchReleased();
	HandleCrouchReleased();
}

void AFPPlayer::Server_HandleCrouchReleased_Implementation()
{
	HandleCrouchReleased();
}

void AFPPlayer::HandleCrouchReleased()
{
	RemovePlayerMovementState(EPlayerMovementState::Crouch);
	ApplyChangesToCharacter();
}

void AFPPlayer::UpdateCharacterSliding(const float DeltaTime)
{
	if (_slideTimer > 0)
	{
		_slideTimer -= DeltaTime;
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

		const auto velocity = GetVelocity().Size();
		if (velocity < MinSlideSpeed)
		{
			StopCharacterSliding();
			RemovePlayerMovementState(EPlayerMovementState::Slide);
			PushPlayerMovementState(EPlayerMovementState::Crouch);
			ApplyChangesToCharacter();
		}
	}
}

void AFPPlayer::StopCharacterSliding()
{
	const FRotator cameraRotation = CameraBoom->GetRelativeRotation();
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);
	const FRotator currentRotation = playerController->GetControlRotation();
	playerController->SetControlRotation(currentRotation + cameraRotation);
	CameraBoom->bUsePawnControlRotation = true;

	_slideTimer = 0;

	RemovePlayerMovementState(EPlayerMovementState::Slide);
	ApplyChangesToCharacter();
}

void AFPPlayer::Client_HandleInteractPressed()
{
	Server_HandleInteractPressed();
	HandleInteractPressed();
}

void AFPPlayer::Server_HandleInteractPressed_Implementation()
{
	HandleInteractPressed();
}

void AFPPlayer::HandleInteractPressed()
{
	if (InteractionDisplayManager == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Interaction NULL");
		return;
	}

	if (_currentInteractionComponent != nullptr)
	{
		_currentInteractionComponent->SetInteractionTime();
		_currentInteractionComponent->StartInteraction();
	}
}

void AFPPlayer::Client_HandleInteractReleased()
{
	Server_HandleInteractReleased();
	HandleInteractReleased();
}

void AFPPlayer::Server_HandleInteractReleased_Implementation()
{
	HandleInteractReleased();
}

void AFPPlayer::HandleInteractReleased()
{
	if (InteractionDisplayManager == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Interaction NULL");
		return;
	}

	if (_currentInteractionComponent != nullptr)
	{
		InteractionDisplayManager->SetInteractionBarProgress(0);
		_currentInteractionComponent->CancelInteraction();

		ClearInteractableObject();
	}
}

void AFPPlayer::UpdateInteractibleCollection(float DeltaTime)
{
	if (InteractionDisplayManager == nullptr)
	{
		return;
	}

	if (_currentInteractionComponent != nullptr && _currentInteractionComponent->IsInteractionActive())
	{
		const auto actor = _currentInteractionComponent->GetOwner();
		const float distance = FVector::Distance(actor->GetActorLocation(), GetActorLocation());
		if (distance > MaxInteractionDistance)
		{
			Client_HandleInteractReleased();
			return;
		}

		const bool interactionComplete = _currentInteractionComponent->InteractionUpdate(DeltaTime);
		if (interactionComplete)
		{
			switch (_currentInteractionComponent->GetInteractibleType())
			{
			case EInteractibleType::Weapon:
				PickupWeapon(Cast<ABaseWeapon>(_currentInteractionComponent->GetOwner()));
				break;
			}

			InteractionDisplayManager->HideInteractionBar();

			ClearInteractableObject();
		}
		else
		{
			InteractionDisplayManager->SetInteractionBarProgress(_currentInteractionComponent->GetInteractionProgress());
		}
	}
	else
	{
		FHitResult hitResult;
		const FVector startPosition = InteractionCastPoint->GetComponentLocation();
		const FVector endPosition = startPosition + CharacterCamera->GetForwardVector() * MaxInteractionDistance;

		const bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility);
		if (hit && hitResult.GetActor() != nullptr)
		{
			AActor* tempActor = hitResult.GetActor();
			UActorComponent* actorComponent = tempActor->GetComponentByClass(UInteractionComponent::StaticClass());
			UInteractionComponent* interactionComponent = Cast<UInteractionComponent>(actorComponent);

			if (interactionComponent != nullptr)
			{
				_currentInteractionComponent = interactionComponent;

				InteractionDisplayManager->ShowInteractionBar();
			}
		}
		else
		{
			InteractionDisplayManager->HideInteractionBar();
		}
	}
}

void AFPPlayer::ClearInteractableObject()
{
	_currentInteractionComponent = nullptr;
}

bool AFPPlayer::IsInFastMovementState()
{
	const EPlayerMovementState movementState = GetTopPlayerState();

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

void AFPPlayer::PushPlayerMovementState(const EPlayerMovementState MovementState)
{
	if (_movementStack.Num() > 0 && GetTopPlayerState() == MovementState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Movement: Previous State And Last State Is Same");
	}

	_movementStack.Push(MovementState);
}

void AFPPlayer::RemovePlayerMovementState(const EPlayerMovementState MovementState)
{
	for (int i = _movementStack.Num() - 1; i >= 0; i--)
	{
		if (_movementStack[i] == MovementState)
		{
			_movementStack.RemoveAt(i);
		}
	}
}

bool AFPPlayer::HasPlayerState(const EPlayerMovementState MovementState)
{
	for (int i = 0; i < _movementStack.Num(); i++)
	{
		if (_movementStack[i] == MovementState)
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

	const FVector relativeLocation = GetMesh()->GetRelativeLocation();
	GetMesh()->SetRelativeLocation(FVector(relativeLocation.X, relativeLocation.Y, DefaultMeshZPosition));

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

			if (_currentWeapon == EPlayerWeapon::Melee)
			{
				const FVector currentLocation = GetMesh()->GetRelativeLocation();
				GetMesh()->SetRelativeLocation(FVector(currentLocation.X, currentLocation.Y, CrouchMeshZPosition));
			}
		}
		break;

	case EPlayerMovementState::Slide:
		{
			GetCharacterMovement()->MaxWalkSpeed = SlideSpeed;
			SetCapsuleData(CrouchHalfHeight, CrouchRadius);

			if (_currentWeapon == EPlayerWeapon::Melee)
			{
				const FVector currentLocation = GetMesh()->GetRelativeLocation();
				GetMesh()->SetRelativeLocation(FVector(currentLocation.X, currentLocation.Y, CrouchMeshZPosition));
			}
		}
		break;

	default:
		break;
	}
}

void AFPPlayer::UpdateLeftRightHandPosition() const
{
	switch (_currentWeapon)
	{
	case EPlayerWeapon::Primary:
		{
			const FVector leftHandLocation = _primaryWeapon->SkeletalWeaponMesh->GetSocketLocation("LeftHandLocation");
			const FVector rightHandLocation = _primaryWeapon->SkeletalWeaponMesh->GetSocketLocation("RightHandLocation");

			CameraLeftHandView->SetWorldLocation(leftHandLocation);
			CameraRightHandView->SetWorldLocation(rightHandLocation);
		}
		break;

	case EPlayerWeapon::Secondary:
		{
			const FVector leftHandLocation = _secondaryWeapon->SkeletalWeaponMesh->GetSocketLocation("LeftHandLocation");
			const FVector rightHandLocation = _secondaryWeapon->SkeletalWeaponMesh->GetSocketLocation("RightHandLocation");

			CameraLeftHandView->SetWorldLocation(leftHandLocation);
			CameraRightHandView->SetWorldLocation(rightHandLocation);
		}
		break;

	case EPlayerWeapon::Melee:
	default:
		CameraLeftHandView->SetRelativeLocation(LeftMeleeDefaultPosition);
		CameraRightHandView->SetRelativeLocation(RightMeleeDefaultPosition);
		break;
	}
}

void AFPPlayer::IKFootTrace(FName SocketName, float Distance, FVector& OutHitLocation, float& FootTraceOffset)
{
	const FVector socketLocation = GetMesh()->GetSocketLocation(SocketName);
	const FVector actorLocation = GetActorLocation();

	const FVector startLocation = FVector(socketLocation.X, socketLocation.Y, actorLocation.Z);
	const FVector endLocation = FVector(socketLocation.X, socketLocation.Y, Distance);

	FHitResult hitResult;
	const bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility);

	if (hit)
	{
		const FVector hitLocation = hitResult.Location;
		const FVector meshLocation = GetMesh()->GetComponentLocation();
		const FVector targetLocation = hitLocation - meshLocation;

		FootTraceOffset = targetLocation.Z - IkHipOffset;
		OutHitLocation = hitLocation;
	}
	else
	{
		OutHitLocation = FVector::ZeroVector;
		FootTraceOffset = 0;
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

void AFPPlayer::SetCapsuleData(float TargetHeight, float TargetRadius)
{
	_capsuleHeight = FVector2D(TargetHeight, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	_capsuleRadius = FVector2D(TargetRadius, GetCapsuleComponent()->GetUnscaledCapsuleRadius());
	_capsuleLerpAmount = 0;
}

void AFPPlayer::UpdateCapsuleSize(float DeltaTime)
{
	if (_capsuleLerpAmount > 1 || _capsuleLerpAmount < 0)
	{
		CameraBoom->bEnableCameraRotationLag = false;
		return;
	}

	CameraBoom->bEnableCameraRotationLag = true;
	const float currentHeight = FMath::Lerp(_capsuleHeight.Y, _capsuleHeight.X, _capsuleLerpAmount);
	const float currentRadius = FMath::Lerp(_capsuleRadius.Y, _capsuleRadius.X, _capsuleLerpAmount);

	GetCapsuleComponent()->SetCapsuleHalfHeight(currentHeight);
	GetCapsuleComponent()->SetCapsuleRadius(currentRadius);

	_capsuleLerpAmount += CapsuleSizeLerpRate * DeltaTime;
	if (_capsuleLerpAmount > 1)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(_capsuleHeight.X);
		GetCapsuleComponent()->SetCapsuleRadius(_capsuleRadius.X);
	}
}

void AFPPlayer::Client_HandleFirePressed()
{
	Server_HandleFirePressed();
	HandleFirePressed();
}

void AFPPlayer::Server_HandleFirePressed_Implementation()
{
	HandleFirePressed();
}

void AFPPlayer::HandleFirePressed()
{
	_firePressed = true;
}

void AFPPlayer::Client_HandleFireReleased()
{
	Server_HandleFireReleased();
	HandleFireReleased();
}

void AFPPlayer::Server_HandleFireReleased_Implementation()
{
	HandleFireReleased();
}

void AFPPlayer::HandleFireReleased()
{
	_firePressed = false;
}

void AFPPlayer::FireUpdate(const float DeltaTime)
{
	if (_firePressed)
	{
		switch (_currentWeapon)
		{
		case EPlayerWeapon::Melee:
			{
				if (_meleeWeapon->ShootTick(DeltaTime))
				{
					_meleeWeapon->Shoot();
				}
			}
			break;

		case EPlayerWeapon::Primary:
			{
				if (_primaryWeapon->ShootTick(DeltaTime))
				{
					_primaryWeapon->Shoot();

					_receoilDelay = true;
					SpawnWeaponProjectile(_primaryWeapon->GetProjectile(), _primaryWeapon->GetShootingPoint());

					FRecoilOffset recoilOffset = _primaryWeapon->GetCurrentRecoilData();
					if (IsInFastMovementState())
					{
						recoilOffset.offset *= _primaryWeapon->FastMovementRecoilMultiplier;
					}
					const int maxRecoilCount = _primaryWeapon->GetMaxRecoilCount();

					FTimerDelegate timerHandle;
					FTimerHandle unusedHandle;
					timerHandle.BindUFunction(this, FName("DelayedCameraMovement"), _primaryWeapon, recoilOffset, maxRecoilCount);
					GetWorldTimerManager().SetTimer(unusedHandle, timerHandle, RECOIL_CAMERA_DELAY, false);
				}
			}
			break;

		case EPlayerWeapon::Secondary:
			{
				if (_secondaryWeapon->ShootTick(DeltaTime))
				{
					_secondaryWeapon->Shoot();

					_receoilDelay = true;
					SpawnWeaponProjectile(_secondaryWeapon->GetProjectile(), _secondaryWeapon->GetShootingPoint());

					FRecoilOffset recoilOffset = _secondaryWeapon->GetCurrentRecoilData();
					if (IsInFastMovementState())
					{
						recoilOffset.offset *= _secondaryWeapon->FastMovementRecoilMultiplier;
					}
					const int maxRecoilCount = _secondaryWeapon->GetMaxRecoilCount();

					FTimerDelegate timerHandle;
					FTimerHandle unusedHandle;
					timerHandle.BindUFunction(this, FName("DelayedCameraMovement"), _secondaryWeapon, recoilOffset, maxRecoilCount);
					GetWorldTimerManager().SetTimer(unusedHandle, timerHandle, RECOIL_CAMERA_DELAY, false);
				}
			}
			break;
		}
	}

	switch (_currentWeapon)
	{
	case EPlayerWeapon::Melee:
		break;

	case EPlayerWeapon::Primary:
		{
			if (_primaryWeapon->IsReloading())
			{
				const float reloadPercent = _primaryWeapon->GetReloadTime() / _primaryWeapon->ReloadTime;
				UpdateReloadPercentNotify(reloadPercent);
				ShowWeaponReloadNotify();
			}
			else
			{
				HideWeaponReloadNotify();
			}

			const int magAmmo = _primaryWeapon->GetMagAmmo();
			const int totalAmmo = _primaryWeapon->GetTotalAmmo();
			UpdateAmmoCountNotify(magAmmo, totalAmmo);
		}
		break;

	case EPlayerWeapon::Secondary:
		{
			if (_secondaryWeapon->IsReloading())
			{
				const float reloadPercent = _secondaryWeapon->GetReloadTime() / _secondaryWeapon->ReloadTime;
				UpdateReloadPercentNotify(reloadPercent);
				ShowWeaponReloadNotify();
			}
			else
			{
				HideWeaponReloadNotify();
			}

			const int magAmmo = _secondaryWeapon->GetMagAmmo();
			const int totalAmmo = _secondaryWeapon->GetTotalAmmo();
			UpdateAmmoCountNotify(magAmmo, totalAmmo);
		}
		break;

	default:
		break;
	}
}

void AFPPlayer::UpdateRecoilRotation(const float DeltaTime)
{
	if (_recoilLerpAmount > 1 || _recoilLerpAmount < 0 || _receoilDelay)
	{
		return;
	}

	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const FRotator currentRotation = playerController->GetControlRotation();

	const float startLerp = _recoilLerpAmount;
	_recoilLerpAmount += RecoilRotationLerpRate * DeltaTime;
	float endLerp = _recoilLerpAmount;
	if (endLerp > 1)
	{
		endLerp = 1;
	}
	const float difference = endLerp - startLerp;

	const FVector targetVector = _targetRecoilRotation * difference;
	const FRotator targetRotation = currentRotation + FRotator(targetVector.Y, targetVector.X, 0);

	playerController->SetControlRotation(targetRotation);

	if (_recoilLerpAmount > 1)
	{
		_targetRecoilRotation = FVector::ZeroVector;
	}
}

void AFPPlayer::UpdateADSWeaponPoint(const float DeltaTime)
{
	if (_adsLerpAmount >= 1 || _adsLerpAmount < 0)
	{
		return;
	}

	const FVector newPosition = FMath::Lerp(_adsStartPosition, _adsEndPosition, _adsLerpAmount);
	WeaponAttachPoint->SetRelativeLocation(newPosition);

	_adsLerpAmount += ADSLerpSpeed * DeltaTime;

	if (_adsLerpAmount >= 1)
	{
		WeaponAttachPoint->SetRelativeLocation(_adsEndPosition);
	}
}

void AFPPlayer::DelayedCameraMovement(ABaseWeapon* BaseWeapon, const FRecoilOffset RecoilOffset, const int MaxRecoilCount)
{
	WeaponShotCameraShake(BaseWeapon->CameraShake);
	SetRecoilCameraPosition(RecoilOffset, MaxRecoilCount);

	_receoilDelay = false;
}

void AFPPlayer::SetRecoilCameraPosition(FRecoilOffset RecoilOffset, int MaxRecoilCount)
{
	if (RecoilOffset.index == MaxRecoilCount - 1)
	{
		// Don't do anything...
	}
	else
	{
		const FVector2D offset = RecoilOffset.offset;
		_targetRecoilRotation = _targetRecoilRotation * (1 - _recoilLerpAmount) + FVector(offset.X, offset.Y, 0);
		_recoilLerpAmount = 0;
	}
}

void AFPPlayer::SpawnWeaponProjectile(TSubclassOf<class AActor> Projectile, FVector SpawnPoint) const
{
	if (!HasAuthority())
	{
		return;
	}

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

	FRotator spawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, targetPoint);
	GetWorld()->SpawnActor<ATD_BaseProjectile>(Projectile, SpawnPoint, spawnRotation, spawnParams);
}

void AFPPlayer::Client_HandleAltFirePressed()
{
	Server_HandleAltFirePressed();
	HandleAltFirePressed();
}

void AFPPlayer::Server_HandleAltFirePressed_Implementation()
{
	HandleAltFirePressed();
}

void AFPPlayer::HandleAltFirePressed()
{
	switch (_currentWeapon)
	{
	case EPlayerWeapon::Melee:
		{
			if (_meleeWeapon->HasAds)
			{
				_adsStartPosition = WeaponAttachPoint->GetRelativeLocation();
				_adsEndPosition = ADSAttachPointPosition;
				_adsLerpAmount = 0;
			}
		}
		break;

	case EPlayerWeapon::Primary:
		{
			if (_primaryWeapon->HasAds)
			{
				_adsStartPosition = WeaponAttachPoint->GetRelativeLocation();
				_adsEndPosition = ADSAttachPointPosition;
				_adsLerpAmount = 0;
			}
		}
		break;

	case EPlayerWeapon::Secondary:
		{
			if (_secondaryWeapon->HasAds)
			{
				_adsStartPosition = WeaponAttachPoint->GetRelativeLocation();
				_adsEndPosition = ADSAttachPointPosition;
				_adsLerpAmount = 0;
			}
		}
		break;

	default:
		break;
	}
}

void AFPPlayer::Client_HandleAltFireReleased()
{
	Server_HandleAltFireReleased();
	HandleAltFireReleased();
}

void AFPPlayer::Server_HandleAltFireReleased_Implementation()
{
	HandleAltFireReleased();
}

void AFPPlayer::HandleAltFireReleased()
{
	_adsStartPosition = WeaponAttachPoint->GetRelativeLocation();
	_adsEndPosition = NonADSAttachPointPosition;
	_adsLerpAmount = 0;
}

EPlayerWeapon AFPPlayer::GetCurrentWeapon()
{
	return _currentWeapon;
}

void AFPPlayer::ChangeCurrentWeapon(EPlayerWeapon Weapon)
{
	if (_currentWeapon == Weapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Trying to Equip Same Weapon");
	}

	_currentWeapon = Weapon;
	ApplyWeaponChangesToCharacter();
}

void AFPPlayer::PickupWeapon(ABaseWeapon* Weapon)
{
	if (Weapon == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Trying to Pickup NULL Weapon");
		return;
	}

	const EPlayerWeapon weaponType = Weapon->WeaponType;
	switch (weaponType)
	{
	case EPlayerWeapon::Melee:
		// Do nothing as Melee Weapon cannot be changed...
		break;

	case EPlayerWeapon::Primary:
		{
			Weapon->PickupWeapon();
			DisplayWeaponNotify(Weapon->WeaponImage, Weapon->GetMagAmmo(), Weapon->GetTotalAmmo(), Weapon->WeaponName, true);
			PickupPrimaryWeapon(Weapon);
		}
		break;

	case EPlayerWeapon::Secondary:
		{
			Weapon->PickupWeapon();
			DisplayWeaponNotify(Weapon->WeaponImage, Weapon->GetMagAmmo(), Weapon->GetTotalAmmo(), Weapon->WeaponName, false);
			PickupSecondaryWeapon(Weapon);
		}
		break;
	}

	ApplyChangesToCharacter();
}

void AFPPlayer::Client_HandleCheckAndDropWeapon()
{
	Server_HandleCheckAndDropWeapon();
	HandleCheckAndDropWeapons();
}

void AFPPlayer::Server_HandleCheckAndDropWeapon_Implementation()
{
	HandleCheckAndDropWeapons();
}

void AFPPlayer::HandleCheckAndDropWeapons()
{
	switch (_currentWeapon)
	{
	case EPlayerWeapon::Melee:
		// Do nothing as Melee Weapon cannot be changed...
		break;

	case EPlayerWeapon::Primary:
		{
			DropWeaponNotify(true);
			DropPrimaryWeapon();
		}
		break;

	case EPlayerWeapon::Secondary:
		{
			DropWeaponNotify(false);
			DropSecondaryWeapon();
		}
		break;
	}
}

ABaseWeapon* AFPPlayer::GetPrimaryWeapon()
{
	return _primaryWeapon;
}

void AFPPlayer::PickupPrimaryWeapon(ABaseWeapon* PrimaryWeapon)
{
	if (_primaryWeapon != nullptr)
	{
		DropPrimaryWeapon();
	}

	_primaryWeapon = PrimaryWeapon;
	ChangeCurrentWeapon(EPlayerWeapon::Primary);

	const FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
	                                                                            EAttachmentRule::SnapToTarget,
	                                                                            EAttachmentRule::KeepWorld,
	                                                                            false);
	_primaryWeapon->AttachToComponent(WeaponAttachPoint, attachmentRules);
}

ABaseWeapon* AFPPlayer::DropPrimaryWeapon()
{
	_primaryWeapon->DropWeapon();

	const auto weaponCopy = _primaryWeapon;
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

void AFPPlayer::PickupSecondaryWeapon(ABaseWeapon* SecondaryWeapon)
{
	if (_secondaryWeapon != nullptr)
	{
		DropSecondaryWeapon();
	}

	_secondaryWeapon = SecondaryWeapon;
	ChangeCurrentWeapon(EPlayerWeapon::Secondary);

	const FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
	                                                                            EAttachmentRule::SnapToTarget,
	                                                                            EAttachmentRule::KeepRelative,
	                                                                            true);
	_secondaryWeapon->AttachToComponent(WeaponAttachPoint, attachmentRules);
}

ABaseWeapon* AFPPlayer::DropSecondaryWeapon()
{
	_secondaryWeapon->DropWeapon();

	const auto weaponCopy = _secondaryWeapon;
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
	return _secondaryWeapon != nullptr;
}

ABaseWeapon* AFPPlayer::GetMeleeWeapon()
{
	return _meleeWeapon;
}

bool AFPPlayer::HasMeleeWeapon()
{
	return _meleeWeapon != nullptr;
}

void AFPPlayer::PickupMeleeWeapon(ABaseWeapon* i_MeleeWeapon)
{
	if (_meleeWeapon != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Trying to Pickup Second Melee Weapon!!!");
		return;
	}

	_meleeWeapon = i_MeleeWeapon;
	ChangeCurrentWeapon(EPlayerWeapon::Melee);

	const FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
	                                                                            EAttachmentRule::SnapToTarget,
	                                                                            EAttachmentRule::KeepRelative,
	                                                                            true);
	_meleeWeapon->AttachToComponent(WeaponAttachPoint, attachmentRules);
}

void AFPPlayer::ApplyWeaponChangesToCharacter() const
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

void AFPPlayer::Client_HandleMeleeSelected()
{
	Server_HandleMeleeSelected();
	HandleMeleeSelected();
}

void AFPPlayer::Server_HandleMeleeSelected_Implementation()
{
	HandleMeleeSelected();
}

void AFPPlayer::HandleMeleeSelected()
{
	ChangeCurrentWeapon(EPlayerWeapon::Melee);
	ApplyWeaponChangesToCharacter();
}

void AFPPlayer::Client_HandlePrimarySelected()
{
	Server_HandlePrimarySelected();
	HandlePrimarySelected();
}

void AFPPlayer::Server_HandlePrimarySelected_Implementation()
{
	HandlePrimarySelected();
}

void AFPPlayer::HandlePrimarySelected()
{
	if (HasPrimaryWeapon())
	{
		ChangeCurrentWeapon(EPlayerWeapon::Primary);
		ApplyWeaponChangesToCharacter();
	}
}

void AFPPlayer::Client_HandleSecondarySelected()
{
	Server_HandleSecondarySelected();
	HandleSecondarySelected();
}

void AFPPlayer::Server_HandleSecondarySelected_Implementation()
{
	HandleSecondarySelected();
}

void AFPPlayer::HandleSecondarySelected()
{
	if (HasSecondaryWeapon())
	{
		ChangeCurrentWeapon(EPlayerWeapon::Secondary);
		ApplyWeaponChangesToCharacter();
	}
}

void AFPPlayer::Client_HandlePlayerPinged()
{
	Server_HandlePlayerPinged();
}

void AFPPlayer::Server_HandlePlayerPinged_Implementation()
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

void AFPPlayer::WallClimbCheck(float DeltaTime)
{
	// FHitResult hitResult;
	// FCollisionQueryParams collisionParams;
	// collisionParams.AddIgnoredActor(this);
	//
	// FVector startPosition = GetActorLocation();
	// FVector endPosition = WallCheckPoint->GetComponentLocation();
	//
	// bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility, collisionParams);
	// if (hit && hitResult.GetActor() != nullptr && !hitResult.GetActor()->ActorHasTag(NotClimbableTag))
	// {
	// 	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// 	FKey key = EKeys::SpaceBar;
	//
	// 	float duration = playerController->GetInputKeyTimeDown(key); // TODO: Change this when custom input is added...
	// 	if (duration != 0)
	// 	{
	// 		_currentClimbTime += DeltaTime;
	// 		if (_currentClimbTime <= ClimbDuration)
	// 		{
	// 			_isClimbing = true;
	// 			LaunchCharacter(FVector(0, 0, ClimbVelocity), true, true);
	// 		}
	// 		else if (_isOnGround)
	// 		{
	// 			_isClimbing = false;
	// 			_currentClimbTime = 0;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		_isClimbing = false;
	// 		_currentClimbTime = 0;
	// 	}
	// }
	// else
	// {
	// 	_isClimbing = false;
	// 	_currentClimbTime = 0;
	// }
	//
	// // So that when climbing the jump state is not pushed
	// if (_isClimbing)
	// {
	// 	RemovePlayerMovementState(EPlayerMovementState::Jump);
	// 	RemovePlayerMovementState(EPlayerMovementState::RunJump);
	// }
}

bool AFPPlayer::IsClimbing()
{
	return _isClimbing;
}

void AFPPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPPlayer, _movementStack);
	DOREPLIFETIME(AFPPlayer, _verticalInput);
	DOREPLIFETIME(AFPPlayer, _horizontalInput);
	DOREPLIFETIME(AFPPlayer, _isClimbing);
	DOREPLIFETIME(AFPPlayer, _currentWeapon);
	DOREPLIFETIME(AFPPlayer, _meleeWeapon);
	DOREPLIFETIME(AFPPlayer, _primaryWeapon);
	DOREPLIFETIME(AFPPlayer, _secondaryWeapon);
}
