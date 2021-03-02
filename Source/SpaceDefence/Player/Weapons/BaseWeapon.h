// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShake.h"

#include "../../Utils/Enums.h"
#include "../../Utils/Structs.h"

#include "BaseWeapon.generated.h"

struct FSortRecoil
{
	bool operator()(const FRecoilOffset A, const FRecoilOffset B) const
	{
		return A.index < B.index;
	}
};

class UTextAsset;

UCLASS()
class SPACEDEFENCE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

protected:

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

	float _reloadTime;
	int _magAmmoLeft;
	int _totalAmmoLeft;
	void FinishReloading();

	bool _isLeft;
	float _currentRandomXPosition;
	int _currentRandomShotCount;

public:
#pragma region Properties

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly)
	class USkeletalMeshComponent* SkeletalWeaponMesh;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	EPlayerWeapon WeaponType;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	float FireRate;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	bool UseSkeletonMesh;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	bool HasAds;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	int MagSize;

	UPROPERTY(Category = "Weapon|Data", EditAnywhere)
	float ReloadTime;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	bool HasRecoil;

	UPROPERTY(Category = "Weapon|Recoil", BlueprintReadOnly, EditAnywhere)
	UTextAsset* RecoilData;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float FastMovementRecoilMultiplier;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	TSubclassOf<UMatineeCameraShake> CameraShake;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float RecoilOffsetMultiplier;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	bool IsLeftRandomRecoil;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float RandomRecoilXDelta;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float RandomXOffset;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	int RandomEdgeStopCount;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float RandomStopXDelta;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float RandomStopYDelta;

	UPROPERTY(Category = "Weapon|Recoil", EditAnywhere)
	float RecoilResetTime;

	UPROPERTY(Category = "Weapon|Display", EditAnywhere)
	UTexture2D* WeaponImage;

	UPROPERTY(Category = "Weapon|Display", EditAnywhere)
	FString WeaponName;

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

	int GetMagAmmo() const;
	int GetTotalAmmo() const;
	float GetReloadTime() const;
	bool IsReloading() const;
	float GetFireRate() const;

	UFUNCTION(Category = Weapon, BlueprintCallable)
	void LoadRecoilData(FText recoilText);
};
