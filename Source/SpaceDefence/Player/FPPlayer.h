// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../Utils/Enums.h"
#include "../Utils/Structs.h"

#include "FPPlayer.generated.h"

class ABasePlayerProjectile;
class ABaseWeapon;
class IIntfBaseInteractible;
class AInteractionDisplayManager;
class AWorldPingMarker;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerLanded);

UCLASS()
class SPACEDEFENCE_API AFPPlayer : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USkeletalMeshComponent* FpMesh;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* GroundCheckPoint;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* WallCheckPoint;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* WeaponTempShootingPoint;

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

	IIntfBaseInteractible* _currentInteractable;
	AInteractionDisplayManager* _interactionManager;
	void UpdateInteractibleCollection(float deltaTime);
	void SetInteractableObject(IIntfBaseInteractible* callingObject);
	void ClearInteractableObject();

	ABaseWeapon* _meleeWeapon;
	ABaseWeapon* _primaryWeapon;
	ABaseWeapon* _secondaryWeapon;
	EPlayerWeapon _currentWeapon;
	void PickupWeapon(ABaseWeapon* weapon);
	void ChangeCurrentWeapon(EPlayerWeapon weapon);
	void ApplyWeaponChangesToCharacter();

protected:
	virtual void BeginPlay() override;

public:
#pragma region Parameters

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

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CrouchHalfHeight;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float CrouchRadius;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float DefaultHalfSize;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
		float DefaultRadius;

	UPROPERTY(Category = "Player|Interaction", EditAnywhere)
		float MaxInteractionDistance;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
		TSubclassOf<class ABaseWeapon> MeleeWeapon;

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

	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		EPlayerWeapon GetCurrentWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		ABaseWeapon* GetPrimaryWeapon();
	void PickupPrimaryWeapon(ABaseWeapon* primaryWeapon);
	ABaseWeapon* DropPrimaryWeapon();
	bool HasPrimaryWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		ABaseWeapon* GetSecondaryWeapon();
	bool HasSecondaryWeapon();
	void PickupSecondaryWeapon(ABaseWeapon* secondaryWeapon);
	ABaseWeapon* DropSecondaryWeapon();
	UFUNCTION(Category = Weapons, BlueprintCallable, BlueprintPure)
		ABaseWeapon* GetMeleeWeapon();
	bool HasMeleeWeapon();
	void PickupMeleeWeapon(ABaseWeapon* meleeWeapon);

	AFPPlayer();
	virtual void Tick(float DeltaTime) override;
};
