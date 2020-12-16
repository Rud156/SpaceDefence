// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../Utils/Enums.h"

#include "FPPlayer.generated.h"

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

	UPROPERTY(Category = Camera, VisibleDefaultsOnly)
		class UCameraComponent* CharacterCamera;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly)
		class	USpringArmComponent* CameraBoom;

	TArray<EPlayerMovementState> _movementStack;
	float _slideTimer;
	void PushPlayerMovementState(EPlayerMovementState movementState);
	void RemovePlayerMovementState(EPlayerMovementState movementState);
	bool HasPlayerState(EPlayerMovementState movementState);
	void ApplyChangesToCharacter();

	float _currentClimbTime;
	void WallClimbCheck();

	UFUNCTION()
		void FrameDelayedJump();

	bool _isOnGround;
	void UpdateGroundStatus();
	UFUNCTION()
		void PlayerLanded();

	void UpdateCharacterSliding(float deltaTime);
	void StopCharacterSliding();

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void CharacterJump();
	void RunPressed();
	void RunReleased();
	void CrouchPressed();
	void CrouchReleased();

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

	UPROPERTY(Category = "Player|Delegates", BlueprintAssignable)
		FPlayerLanded OnPlayerLanded;

#pragma endregion

	UFUNCTION(Category = "Debug", BlueprintImplementableEvent)
		void MovementStatePushed(EPlayerMovementState playerMovementState);

	AFPPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
