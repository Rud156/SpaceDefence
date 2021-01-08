// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseWeapon.h"
#include "../../Interactibles/IntfBaseInteractible.h"

#include "SecondaryWeapon.generated.h"

class ABasePlayerProjectile;

UCLASS()
class SPACEDEFENCE_API ASecondaryWeapon : public ABaseWeapon, public IIntfBaseInteractible
{
	GENERATED_BODY()

	float _currentInteractionTime;
	float _interactionDuration;
	bool _interactionStarted;

protected:
	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		float WeaponInteractionTime;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		TSubclassOf<class ABasePlayerProjectile> Projectile;

#pragma endregion

	ASecondaryWeapon();
	virtual void Tick(float DeltaTime) override;
	
	virtual TSubclassOf<AActor> GetProjectile() override;

	virtual EInteractibleType GetInteractibleType_Implementation() override;
	virtual bool InteractUpdate_Implementation(float DeltaTime) override;
	virtual void CancelInteraction_Implementation() override;
	virtual bool InteractionStarted_Implementation() override;
	virtual float GetCurrentInteractionTime_Implementation() override;
	virtual float GetBaseInteractionTime_Implementation() override;
	virtual float GetInteractionProgress_Implementation() override;
	virtual void SetInteractionTime_Implementation(float DurationMultiplier) override;
};
