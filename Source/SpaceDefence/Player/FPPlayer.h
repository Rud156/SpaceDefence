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

	UPROPERTY(Category = Camera, VisibleDefaultsOnly)
		class UCameraComponent* CharacterCamera;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly)
		class	USpringArmComponent* CameraBoom;

	PlayerMovementState _playerMovementState;
	PlayerMovementState _delayedMovementState;
	void SetPlayerMovementState(PlayerMovementState movementState);
	void SetDelayedMovementState(PlayerMovementState delayedState);
	void ResetMovementVariables();

	bool _isOnGround;
	void UpdateGroundStatus();
	UFUNCTION()
		void PlayerLanded();

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void CharacterJump();
	void RunPressed();
	void RunReleased();

protected:
	virtual void BeginPlay() override;

public:
#pragma region Parameters

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float WalkSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float RunSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float TurnSpeed;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float LookUpRate;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float HSensitivityMultiplier;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float VSensitivityMultiplier;

	UPROPERTY(Category = "Player|Movement", EditAnywhere)
		float GroundCheckDistance;

	UPROPERTY(Category = "Player|Delegates", BlueprintAssignable)
		FPlayerLanded OnPlayerLanded;

#pragma endregion

	AFPPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
