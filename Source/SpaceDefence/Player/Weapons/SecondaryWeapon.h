// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "../../Interactibles/IntfBaseInteractible.h"
#include "SecondaryWeapon.generated.h"

class ABasePlayerProjectile;
class UInteractionComponent;

UCLASS()
class SPACEDEFENCE_API ASecondaryWeapon : public ABaseWeapon, public IIntfBaseInteractible
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = Interaction, VisibleDefaultsOnly, BlueprintReadOnly)
		class UInteractionComponent* InteractionComponent;

	virtual void BeginPlay() override;

public:
#pragma region Properties

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		TSubclassOf<class ABasePlayerProjectile> Projectile;

#pragma endregion

	ASecondaryWeapon();
	virtual void Tick(float DeltaTime) override;

	virtual TSubclassOf<AActor> GetProjectile() override;
	virtual UInteractionComponent* GetInteractionComponent_Implementation() override;
};
