// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "../Interactibles/IntfBaseInteractible.h"
#include "PrimaryWeapon.generated.h"

class ATD_BaseProjectile;
class UInteractionComponent;

UCLASS()
class SPACEDEFENCE_API APrimaryWeapon : public ABaseWeapon, public IIntfBaseInteractible
{
	GENERATED_BODY()

private:

	float _currentInteractionTime;
	float _interactionDuration;
	bool _interactionStarted;

protected:
	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadOnly)
	class UInteractionComponent* InteractionComponent;

	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	TSubclassOf<class ATD_BaseProjectile> Projectile;

#pragma endregion

	APrimaryWeapon();
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;

	virtual TSubclassOf<AActor> GetProjectile() override;
	virtual UInteractionComponent* GetInteractionComponent_Implementation() override;
};
