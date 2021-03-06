// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShake.h"

#include "../Utils/Enums.h"
#include "../Utils/Structs.h"
#include "SpaceDefence/Placeables/TD_PlaceableSpawnerActor.h"

#include "FPPlayer.generated.h"

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
	const float RECOIL_CAMERA_DELAY = 0.05f;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CharacterCamera;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CameraLeftHandView;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CameraRightHandView;

	UPROPERTY(Category = PlayerHealth, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UHealthAndDamageComp* HealthAndDamage;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
	class USceneComponent* WallCheckPoint;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
	class USceneComponent* InteractionCastPoint;

	UPROPERTY(Category = Weapon, VisibleDefaultsOnly)
	class USceneComponent* WeaponAttachPoint;

	bool IsInFastMovementState();
	void PushPlayerMovementState(EPlayerMovementState MovementState);
	void RemovePlayerMovementState(EPlayerMovementState MovementState);
	bool HasPlayerState(EPlayerMovementState MovementState);
	void ApplyChangesToCharacter();

	FVector2D _capsuleRadius; // X: Target, Y: Current
	FVector2D _capsuleHeight; // X: Target, Y: Current
	float _capsuleLerpAmount;
	void SetCapsuleData(float TargetHeight, float TargetRadius);
	void UpdateCapsuleSize(float DeltaTime);

	float _currentClimbTime;
	void WallClimbCheck(float DeltaTime);

	UFUNCTION()
	void FrameDelayedJump();

	bool _isOnGround;
	void UpdateGroundStatus();
	UFUNCTION()
	void PlayerLanded();

	float _slideTimer;
	void UpdateCharacterSliding(float DeltaTime);
	void StopCharacterSliding();

	bool _firePressed;
	FVector _targetRecoilRotation;
	float _recoilLerpAmount;
	bool _receoilDelay;
	FVector _adsStartPosition;
	FVector _adsEndPosition;
	float _adsLerpAmount;
	void FireUpdate(float DeltaTime);
	void UpdateRecoilRotation(float DeltaTime);
	void UpdateADSWeaponPoint(float DeltaTime);
	void SetRecoilCameraPosition(FRecoilOffset RecoilOffset, int MaxRecoilCount);
	void SpawnWeaponProjectile(TSubclassOf<class AActor> Projectile, FVector SpawnPoint) const;

	UFUNCTION()
	void DelayedCameraMovement(ABaseWeapon* BaseWeapon, FRecoilOffset RecoilOffset, int MaxRecoilCount);
	void UpdateLeftRightHandPosition() const;

	UInteractionComponent* _currentInteractionComponent;
	void UpdateInteractibleCollection(float DeltaTime);
	void ClearInteractableObject();

	void PickupWeapon(ABaseWeapon* Weapon);
	void ChangeCurrentWeapon(EPlayerWeapon Weapon);
	void ApplyWeaponChangesToCharacter() const;

protected:
	virtual void BeginPlay() override;

public:
#pragma region Parameters

	UPROPERTY(Replicated)
	TArray<EPlayerMovementState> _movementStack;
	UPROPERTY(Replicated)
	float _verticalInput;
	UPROPERTY(Replicated)
	float _horizontalInput;
	UPROPERTY(Replicated)
	bool _isClimbing;
	UPROPERTY(Replicated)
	EPlayerWeapon _currentWeapon;
	UPROPERTY(Replicated)
	ABaseWeapon* _meleeWeapon;
	UPROPERTY(Replicated)
	ABaseWeapon* _primaryWeapon;
	UPROPERTY(Replicated)
	ABaseWeapon* _secondaryWeapon;

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
	float FallVelocityThreshold;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float CrouchHalfHeight;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float CrouchRadius;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float DefaultHalfHeight;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float DefaultRadius;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float CapsuleSizeLerpRate;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float DefaultMeshZPosition;

	UPROPERTY(Category = "Player|Size", EditAnywhere)
	float CrouchMeshZPosition;

	UPROPERTY(Category = "Player|IK", EditAnywhere)
	FVector LeftMeleeDefaultPosition;

	UPROPERTY(Category = "Player|IK", EditAnywhere)
	FVector RightMeleeDefaultPosition;

	UPROPERTY(Category = "Player|IK", BlueprintReadWrite, Replicated)
	float IkHipOffset;

	UPROPERTY(Category = "Player|Interaction", EditAnywhere)
	float MaxInteractionDistance;

	UPROPERTY(Category = "Player|Interaction", BlueprintReadWrite)
	AInteractionDisplayManager* InteractionDisplayManager;

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

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
	float RecoilRotationLerpRate;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
	FVector NonADSAttachPointPosition;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
	FVector ADSAttachPointPosition;

	UPROPERTY(Category = "Player|Weapon", EditAnywhere)
	float ADSLerpSpeed;

	UPROPERTY(Category = "Player|Delegates", BlueprintAssignable)
	FPlayerLanded OnPlayerLanded;

	UPROPERTY(Category = "Player|SpawnerSettings", BlueprintReadWrite)
	ATD_PlaceableSpawnerActor* PlaceAbleSpawnerActorRef = nullptr;
#pragma endregion

#pragma region Inputs

	void Client_MoveForward(const float Value);
	UFUNCTION(Server, Reliable)
	void Server_MoveForward(const float Value);
	void MoveForward(const float Value);

	void Client_MoveRight(const float Value);
	UFUNCTION(Server, Reliable)
	void Server_MoveRight(const float Value);
	void MoveRight(const float Value);

	void Client_Turn(const float Value);
	UFUNCTION(Server, Reliable)
	void Server_Turn(const float Value);
	void Turn(const float Value);

	void Client_LookUp(const float Value);
	UFUNCTION(Server, Reliable)
	void Server_LookUp(const float Value);
	void LookUp(const float Value);

	void Client_HandleJumpPressed();
	UFUNCTION(Server, Reliable)
	void Server_HandleJumpPressed();
	void HandleJumpPressed();

	void Client_HandleRunPressed();
	UFUNCTION(Server, Reliable)
	void Server_HandleRunPressed();
	void HandleRunPressed();

	void Client_HandleRunReleased();
	UFUNCTION(Server, Reliable)
	void Server_HandleRunReleased();
	void HandleRunReleased();

	void Client_HandleCrouchPressed();
	UFUNCTION(Server, Reliable)
	void Server_HandleCrouchPressed();
	void HandleCrouchPressed();

	void Client_HandleCrouchReleased();
	UFUNCTION(Server, Reliable)
	void Server_HandleCrouchReleased();
	void HandleCrouchReleased();

	void Client_HandleFirePressed();
	UFUNCTION(Server, Reliable)
	void Server_HandleFirePressed();
	void HandleFirePressed();

	void Client_HandleFireReleased();
	UFUNCTION(Server, Reliable)
	void Server_HandleFireReleased();
	void HandleFireReleased();

	void Client_HandleAltFirePressed();
	UFUNCTION(Server, Reliable)
	void Server_HandleAltFirePressed();
	void HandleAltFirePressed();

	void Client_HandleAltFireReleased();
	UFUNCTION(Server, Reliable)
	void Server_HandleAltFireReleased();
	void HandleAltFireReleased();

	void Client_HandleInteractPressed();
	UFUNCTION(Server, Reliable)
	void Server_HandleInteractPressed();
	void HandleInteractPressed();

	void Client_HandleInteractReleased();
	UFUNCTION(Server, Reliable)
	void Server_HandleInteractReleased();
	void HandleInteractReleased();

	void Client_HandleMeleeSelected();
	UFUNCTION(Server, Reliable)
	void Server_HandleMeleeSelected();
	void HandleMeleeSelected();

	void Client_HandlePrimarySelected();
	UFUNCTION(Server, Reliable)
	void Server_HandlePrimarySelected();
	void HandlePrimarySelected();

	void Client_HandleSecondarySelected();
	UFUNCTION(Server, Reliable)
	void Server_HandleSecondarySelected();
	void HandleSecondarySelected();

	void Client_HandleCheckAndDropWeapon();
	UFUNCTION(Server, Reliable)
	void Server_HandleCheckAndDropWeapon();
	void HandleCheckAndDropWeapons();

	void Client_HandlePlayerPinged();
	UFUNCTION(Server, Reliable)
	void Server_HandlePlayerPinged();

#pragma endregion

	UFUNCTION(Category = "Debug", BlueprintImplementableEvent)
	void MovementStatePushed(EPlayerMovementState PlayerMovementState);
	UFUNCTION(Category = "Player|MovementState", BlueprintImplementableEvent)
	void PlayerJumped();
	UFUNCTION(Category = "Player|MovementState", BlueprintImplementableEvent)
	void PlayerRunJumped();
	UFUNCTION(Category = "Player|MovementState", BlueprintImplementableEvent)
	void PlayerRunStarted();
	UFUNCTION(Category = "Player|MovementState", BlueprintImplementableEvent)
	void PlayerRunEnded();
	UFUNCTION(Category = "Player|MovementState", BlueprintImplementableEvent)
	void WeaponShotCameraShake(TSubclassOf<UMatineeCameraShake> CameraShake);

	UFUNCTION(Category = "Player|Weapon", BlueprintImplementableEvent)
	void ShowWeaponReloadNotify();
	UFUNCTION(Category = "Player|Weapon", BlueprintImplementableEvent)
	void UpdateReloadPercentNotify(float Percent);
	UFUNCTION(Category = "Player|Weapon", BlueprintImplementableEvent)
	void HideWeaponReloadNotify();
	UFUNCTION(Category = "Player|Weapon", BlueprintImplementableEvent)
	void UpdateAmmoCountNotify(int MagAmmo, int TotalAmmo);
	UFUNCTION(Category = "Player|Weapon", BlueprintImplementableEvent)
	void DropWeaponNotify(bool IsPrimary);
	UFUNCTION(Category = "Player|Weapon", BlueprintImplementableEvent)
	void DisplayWeaponNotify(UTexture2D* WeaponTexture, int MagAmmo, int TotalAmmo, const FString& WeaponName, bool IsPrimary);

	UFUNCTION(Category = "Player|IK", BlueprintCallable)
	void IKFootTrace(FName SocketName, float Distance, FVector& OutHitLocation, float& FootTraceOffset);

	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	EPlayerWeapon GetCurrentWeapon();
	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	ABaseWeapon* GetPrimaryWeapon();
	void PickupPrimaryWeapon(ABaseWeapon* PrimaryWeapon);
	ABaseWeapon* DropPrimaryWeapon();
	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	bool HasPrimaryWeapon();
	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	ABaseWeapon* GetSecondaryWeapon();
	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	bool HasSecondaryWeapon();
	void PickupSecondaryWeapon(ABaseWeapon* SecondaryWeapon);
	ABaseWeapon* DropSecondaryWeapon();
	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	ABaseWeapon* GetMeleeWeapon();
	UFUNCTION(Category = "Player|Weapons", BlueprintCallable, BlueprintPure)
	bool HasMeleeWeapon();
	void PickupMeleeWeapon(ABaseWeapon* i_MeleeWeapon);

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

	AFPPlayer(const class FObjectInitializer& PCIP);
	virtual void Tick(float DeltaTime) override;
};
