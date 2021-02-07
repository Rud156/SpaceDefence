// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../../Utils/Enums.h"
#include "../../Utils/Structs.h"

#include "BaseWeapon.generated.h"

struct FSortRecoil
{
	bool operator() (const FRecoilOffset A, const FRecoilOffset B) const
	{
		return A.index < B.index;
	}
};

class UTextAsset;

UCLASS()
class SPACEDEFENCE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

		float BASE_RECOIL_MULTIPLIER = 0.1f;

protected:

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class UStaticMeshComponent* StaticWeaponMesh;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USkeletalMeshComponent* SkeletalWeaponMesh;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class USceneComponent* ShootingPoint;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
		class UBoxComponent* WeaponCollider;

	virtual void BeginPlay() override;

	float _lastShotTime;
	TArray<FRecoilOffset> _recoilOffsets;
	int _currentRecoilIndex;
	float _currentRecoilTime;
	void RecoilTick(float deltaTime);

	bool _isLeft;
	int _currentRandomIndex;
	int _currentRandomShotCount;

public:
#pragma region Properties

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		EPlayerWeapon WeaponType;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		float FireRate;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		bool HasRecoil;

	UPROPERTY(Category = "Weapon|Data", BlueprintReadOnly, EditAnywhere)
		UTextAsset* RecoilData;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		int RandomXOffset;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		int RandomXStopCount;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
		float RecoilResetTime;

#pragma endregion

	ABaseWeapon();
	virtual void Tick(float DeltaTime) override;

	virtual FVector GetShootingPoint();
	virtual void Shoot();

	virtual void HideWeapon();
	virtual void ShowWeapon();

	virtual void PickupWeapon();
	virtual void DropWeapon();

	virtual bool ShootTick(float DeltaTime);
	virtual TSubclassOf<class AActor> GetProjectile();
	virtual FRecoilOffset GetCurrentRecoilData();
	virtual int GetMaxRecoilCount();

	UFUNCTION(Category = Weapon, BlueprintCallable)
		void LoadRecoilData(FText recoilText);
};