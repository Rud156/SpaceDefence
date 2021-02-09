// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../Utils/Enums.h"
#include "../Utils/Structs.h"

#include "FPPlayer.generated.h"

class ABasePlayerProjectile;
class ABaseWeapon;
class UInteractionComponent;
class AInteractionDisplayManager;
class AWorldPingMarker;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerLanded);

UCLASS()
class SPACEDEFENCE_API AFPPlayer : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* GroundCheckPoint;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* WallCheckPoint;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* InteractionCastPoint;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly)
		class UCameraComponent* CharacterCamera;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Weapon, VisibleDefaultsOnly)
		class USceneComponent* WeaponAttachPoint;

	TArray<EPlayerMovementState> _movementStack;
	float _slideTimer;
	void PushPlayerMovementState(EPlayerMovementState movementState);
	void RemovePlayerMovementState(EPlayerMovementState movementState);
	bool HasPlayerState(EPlayerMovementState movementState);
	void ApplyChangesToCharacter();

	float _verticalInput;
	float _horizontalInput;

	FVector2D _capsuleRadius; // X: Target, Y: Current
	FVector2D _capsuleHeight; // X: Target, Y: Current
	FVector2D _meshZPosition; // X: Target, Y: Current
	float _lerpAmount;
	float _currentCrouchZPosition;
	float _currentDefaultZPosition;
	void ResetMeshNonWeaponState();
	void ResetMeshWeaponState();
	void SetCapsuleData(float targetHeight, float targetRadius, float targetZPosition);
	void UpdateCapsuleSize(float deltaTime);

	float _currentClimbTime;
	bool _isClimbing;
	void WallClimbCheck(float deltaTime);

	UFUNCTION()
		void FrameDelayedJump();

	bool _isOnGround;
	void UpdateGroundStatus();
	UFUNCTION()
		void PlayerLanded();

	void UpdateCharacterSliding(float deltaTime);
	void StopCharacterSliding();

	bool _firePressed;
	void FireUpdate(float deltaTime);
	void UpdateRecoilCamera(FRecoilOffset recoilOffset, int maxRecoilCount);
	UFUNCTION()
		void SpawnWeaponProjectile(TSubclassOf<class AActor> projectile, FVector spawnPoint);

	UInteractionComponent* _currentInteractionComponent;
	AInteractionDisplayManager* _interactionManager;
	void UpdateInteractibleCollection(float deltaTime);
	void ClearInteractableObject();

	ABaseWeapon* _meleeWeapon;
	ABaseWeapon* _primaryWeapon;
	ABaseWeapon* _secondaryWeapon;
	EPlayerWeapon _currentWeapon;
	void PickupWeapon(ABaseWeapon* weapon);
	void ChangeCurrentWeapon(EPlayerWeapon weapon);
	void ApplyWeaponChangesToCharacter();

protected:
	UPROPERTY(Category = PlayerHealth, VisibleAnywhere, BlueprintReadOnly)
		class UHealthAndDamageComp* HealthAndDamage;

	virtual void BeginPlay() override;

public:
#pragma region Parameters

	UPROPERTY(Category = Mesh, BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float WalkSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float RunSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float CrouchSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float SlideSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float SlideDuration;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float MinSlideSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float TurnSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float LookUpRate;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		FVector JumpVelocity;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float ClimbDuration;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float ClimbVelocity;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		FName NotClimbableTag;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float HSensitivityMultiplier;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float VSensitivityMultiplier;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float GroundCheckDistance;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float FallVelocityThreshold;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		FVector2D DefaultMeshPosition;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		FVector2D WeaponMeshPosition;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CrouchHalfHeight;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CrouchRadius;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CrouchMeshZPosition;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CrouchMeshZPositionWeapon;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float DefaultHalfHeight;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float DefaultRadius;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float DefaultMeshZPosition;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float DefaultMeshZPositionWeapon;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CameraDefaultZPosition;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CameraZPositionWeapon;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CapsuleSizeLerpRate;

	UPROPERTY(Category = "Player|Interaction", EditAnywhere)
		float MaxInteractionDistance;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
		TSubclassOf<class ABaseWeapon> MeleeWeapon;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
		FVector AttachRelativeLocation;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
		FRotator AttachRelativeRotation;

	UPROPERTY(Category = "Player|Ping", EditAnywhere)
		TSubclassOf<class AWorldPingMarker> PingMarker;

	UPROPERTY(Category = "Player|Ping", EditAnywhere)
		FVector PingSpawnOffset;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
		float MaxShootRayCastRange;

	UPROPERTY(Category = "Player|Delegates", BlueprintAssignable)
		FPlayerLanded OnPlayerLanded;

#pragma endregion

#pragma region Inputs

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);

	void CharacterJump();
	void RunPressed();
	void RunReleased();
	void CrouchPressed();
	void CrouchReleased();
	void FirePressed();
	void FireReleased();
	void HandleInteractPressed();
	void HandleInteractReleased();
	void HandleMeleeSelected();
	void HandlePrimarySelected();
	void HandleSecondarySelected();
	void CheckAndDropWeapon();
	void HandlePlayerPinged();

#pragma endregion

	UFUNCTION(Category = "Debug", BlueprintImplementableEvent)
		void MovementStatePushed(EPlayerMovementState playerMovementState);
	UFUNCTION(Category = "PlayerMovementState", BlueprintImplementableEvent)
		void PlayerJumped();
	UFUNCTION(Category = "PlayerMovementState", BlueprintImplementableEvent)
		void PlayerRunJumped();
	UFUNCTION(Category = "PlayerMovementState", BlueprintImplementableEvent)
		void PlayerRunStarted();
	UFUNCTION(Category = "PlayerMovementState", BlueprintImplementableEvent)
		void PlayerRunEnded();

	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		EPlayerWeapon GetCurrentWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		ABaseWeapon* GetPrimaryWeapon();
	void PickupPrimaryWeapon(ABaseWeapon* primaryWeapon);
	ABaseWeapon* DropPrimaryWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		bool HasPrimaryWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		ABaseWeapon* GetSecondaryWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		bool HasSecondaryWeapon();
	void PickupSecondaryWeapon(ABaseWeapon* secondaryWeapon);
	ABaseWeapon* DropSecondaryWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		ABaseWeapon* GetMeleeWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		bool HasMeleeWeapon();
	void PickupMeleeWeapon(ABaseWeapon* meleeWeapon);

	UFUNCTION(Category = "Player|Input", BlueprintCallable, BlueprintPure)
		float GetVerticalInput();
	UFUNCTION(Category = "Player|Input", BlueprintCallable, BlueprintPure)
		float GetHorizontalInput();
	UFUNCTION(Category = "Player|State", BlueprintCallable, BlueprintPure)
		bool IsRunning(); // This is different from the running state. Probably only used for effects and animation
	UFUNCTION(Category = "Player|State", BlueprintCallable, BlueprintPure)
		bool IsOnGround();
	UFUNCTION(Category = "Player|State", BlueprintCallable, BlueprintPure)
		bool IsClimbing();
	UFUNCTION(Category = "Player|State", BlueprintCallable, BlueprintPure)
		bool IsFalling();
	UFUNCTION(Category = "Player|State", BlueprintCallable, BlueprintPure)
		EPlayerMovementState GetTopPlayerState();

	AFPPlayer();
	virtual void Tick(float DeltaTime) override;
};
