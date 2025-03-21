// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Containers/UnrealString.h"
#include "Misc/DefaultValueHelper.h"

ABaseWeapon::ABaseWeapon()
{
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	SkeletalWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalWeaponMesh"));
	ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingPoint"));

	RootComponent = WeaponCollider;
	SkeletalWeaponMesh->SetupAttachment(WeaponCollider);
	ShootingPoint->SetupAttachment(WeaponCollider);

	HasRecoil = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (UseSkeletonMesh)
	{
		ShootingPoint->AttachToComponent(SkeletalWeaponMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("ShootingPoint"));
	}
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasRecoil)
	{
		RecoilTick(DeltaTime);
	}

	if (_reloadTime > 0)
	{
		_reloadTime -= DeltaTime;
		if (_reloadTime <= 0)
		{
			FinishReloading();
		}
	}
}

void ABaseWeapon::LoadRecoilData(FText recoilText)
{
	if (!HasRecoil)
	{
		return;
	}

	_recoilOffsets = TArray<FRecoilOffset>();
	FString recoilString = recoilText.ToString();
	FString currentNumberString = "";

	int currentRowIndex = 0;
	int currentColumnIndex = 0;

	for (int i = 0; i < recoilString.Len(); i++)
	{
		const auto letter = recoilString[i];

		if (letter == '\r')
		{
			currentRowIndex += 1;
			currentColumnIndex = 0;
			i += 1;
		}
		else if (letter == '\n')
		{
			currentRowIndex += 1;
			currentColumnIndex = 0;
		}
		else if (letter == ',')
		{
			if (currentNumberString != "")
			{
				int number;
				const bool success = FDefaultValueHelper::ParseInt(currentNumberString, number);
				if (success)
				{
					FRecoilOffset recoilOffset;
					recoilOffset.index = number;
					recoilOffset.rowIndex = currentRowIndex;
					recoilOffset.columnIndex = currentColumnIndex;
					recoilOffset.offset = FVector2D::ZeroVector;

					_recoilOffsets.Add(recoilOffset);
				}

				currentNumberString = "";
			}

			currentColumnIndex += 1;
		}
		else
		{
			currentNumberString += letter;
		}
	}

	const int centerRow = currentRowIndex / 2;
	const int centerColumn = currentColumnIndex / 2;

	for (int j = 0; j < _recoilOffsets.Num(); j++)
	{
		auto recoilData = _recoilOffsets[j];

		const int rowDiff = centerRow - recoilData.rowIndex;
		const int columnDiff = centerColumn - recoilData.columnIndex;

		const FVector2D offset = FVector2D(columnDiff, rowDiff);
		recoilData.offset = offset;

		_recoilOffsets[j] = recoilData;
	}
	_recoilOffsets.Sort(FSortRecoil());
}

bool ABaseWeapon::ShootTick(float DeltaTime)
{
	if (_reloadTime > 0)
	{
		return false;
	}

	const float currentTime = GetWorld()->GetTimeSeconds();
	const float difference = currentTime - _lastShotTime;
	if (difference > FireRate && _magAmmoLeft > 0)
	{
		_lastShotTime = currentTime;
		return true;
	}

	return false;
}

void ABaseWeapon::RecoilTick(float deltaTime)
{
	if (_currentRecoilTime > 0)
	{
		_currentRecoilTime -= deltaTime;
		if (_currentRecoilTime <= 0)
		{
			_currentRecoilIndex = 0;
			_currentRandomXPosition = 0;
			_currentRandomShotCount = 0;
		}
	}
}

FVector ABaseWeapon::GetShootingPoint()
{
	return ShootingPoint->GetComponentLocation();
}

void ABaseWeapon::Shoot()
{
	// Probably use this to play VFX or something...
}

void ABaseWeapon::FinishReloading()
{
	if (_totalAmmoLeft >= MagSize)
	{
		_totalAmmoLeft -= MagSize;
		_magAmmoLeft = MagSize;
	}
	else
	{
		_totalAmmoLeft = 0;
		_magAmmoLeft = _totalAmmoLeft;
	}
}

TSubclassOf<AActor> ABaseWeapon::GetProjectile()
{
	return nullptr;
}

FRecoilOffset ABaseWeapon::GetCurrentRecoilData()
{
	if (!HasRecoil)
	{
		FRecoilOffset recoilData;
		recoilData.columnIndex = 0;
		recoilData.rowIndex = 0;
		recoilData.index = 0;
		recoilData.offset = FVector2D(0, 0);

		return recoilData;
	}

	FRecoilOffset recoilData;
	if (_currentRecoilIndex >= _recoilOffsets.Num())
	{
		recoilData.columnIndex = -1;
		recoilData.rowIndex = -1;
		recoilData.index = -1;

		if (IsLeftRandomRecoil)
		{
			if (_isLeft)
			{
				if (_currentRandomXPosition <= -RandomXOffset)
				{
					_currentRandomShotCount += 1;

					if (_currentRandomShotCount >= RandomEdgeStopCount)
					{
						_currentRandomShotCount = 0;
						_isLeft = false;
						_currentRandomXPosition += RandomRecoilXDelta;
					}

					const float randomXOffset = FMath::RandRange(-RandomStopXDelta, RandomStopXDelta);
					const float randomYOffset = FMath::RandRange(-RandomStopYDelta, RandomStopYDelta);
					recoilData.offset = FVector2D(randomXOffset, randomYOffset);
				}
				else
				{
					recoilData.offset = FVector2D(-FMath::Abs(_currentRandomXPosition), 0);
					_currentRandomXPosition -= RandomRecoilXDelta;

					if (_currentRandomXPosition < -RandomXOffset)
					{
						_currentRandomXPosition = -RandomXOffset;
					}
				}
			}
			else
			{
				if (_currentRandomXPosition >= 0)
				{
					_currentRandomShotCount += 1;

					if (_currentRandomShotCount >= RandomEdgeStopCount)
					{
						_currentRandomShotCount = 0;
						_isLeft = true;
						_currentRandomXPosition -= RandomRecoilXDelta;
					}

					const float randomXOffset = FMath::RandRange(-RandomStopXDelta, RandomStopXDelta);
					const float randomYOffset = FMath::RandRange(-RandomStopYDelta, RandomStopYDelta);
					recoilData.offset = FVector2D(randomXOffset, randomYOffset);
				}
				else
				{
					recoilData.offset = FVector2D(FMath::Abs(_currentRandomXPosition), 0);
					_currentRandomXPosition += RandomRecoilXDelta;

					if (_currentRandomXPosition > 0)
					{
						_currentRandomXPosition = 0;
					}
				}
			}
		}
		else
		{
			if (_isLeft)
			{
				if (_currentRandomXPosition <= 0)
				{
					_currentRandomShotCount += 1;

					if (_currentRandomShotCount >= RandomEdgeStopCount)
					{
						_currentRandomShotCount = 0;
						_isLeft = false;
						_currentRandomXPosition += RandomRecoilXDelta;
					}

					const float randomXOffset = FMath::RandRange(-RandomStopXDelta, RandomStopXDelta);
					const float randomYOffset = FMath::RandRange(-RandomStopYDelta, RandomStopYDelta);
					recoilData.offset = FVector2D(randomXOffset, randomYOffset);
				}
				else
				{
					recoilData.offset = FVector2D(-FMath::Abs(_currentRandomXPosition), 0);
					_currentRandomXPosition -= RandomRecoilXDelta;

					if (_currentRandomXPosition < 0)
					{
						_currentRandomXPosition = 0;
					}
				}
			}
			else
			{
				if (_currentRandomXPosition >= RandomXOffset)
				{
					_currentRandomShotCount += 1;

					if (_currentRandomShotCount >= RandomEdgeStopCount)
					{
						_currentRandomShotCount = 0;
						_isLeft = true;
						_currentRandomXPosition -= RandomRecoilXDelta;
					}

					const float randomXOffset = FMath::RandRange(-RandomStopXDelta, RandomStopXDelta);
					const float randomYOffset = FMath::RandRange(-RandomStopYDelta, RandomStopYDelta);
					recoilData.offset = FVector2D(randomXOffset, randomYOffset);
				}
				else
				{
					recoilData.offset = FVector2D(FMath::Abs(_currentRandomXPosition), 0);
					_currentRandomXPosition += RandomRecoilXDelta;

					if (_currentRandomXPosition > RandomXOffset)
					{
						_currentRandomXPosition = RandomXOffset;
					}
				}
			}
		}

		_currentRecoilIndex = _recoilOffsets.Num() - 1;
	}
	else
	{
		recoilData = _recoilOffsets[_currentRecoilIndex];
		recoilData.offset.X *= RecoilOffsetMultiplier;
		recoilData.offset.Y *= RecoilOffsetMultiplier;

		if (IsLeftRandomRecoil)
		{
			_isLeft = true;
		}
		else
		{
			_isLeft = false;
		}
	}

	_currentRecoilIndex += 1;
	_currentRecoilTime = RecoilResetTime;

	return recoilData;
}

int ABaseWeapon::GetMaxRecoilCount()
{
	if (!HasRecoil)
	{
		return 0;
	}

	return _recoilOffsets.Num();
}

int ABaseWeapon::GetMagAmmo() const
{
	return _magAmmoLeft;
}

int ABaseWeapon::GetTotalAmmo() const
{
	return _totalAmmoLeft;
}

float ABaseWeapon::GetReloadTime() const
{
	return _reloadTime;
}

bool ABaseWeapon::IsReloading() const
{
	return _reloadTime > 0;
}

float ABaseWeapon::GetFireRate() const
{
	return FireRate;
}

void ABaseWeapon::HideWeapon()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ABaseWeapon::ShowWeapon()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ABaseWeapon::PickupWeapon()
{
	WeaponCollider->SetSimulatePhysics(false);
	WeaponCollider->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABaseWeapon::DropWeapon()
{
	const FDetachmentTransformRules detachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
	                                                                        EDetachmentRule::KeepWorld,
	                                                                        EDetachmentRule::KeepWorld,
	                                                                        true);
	DetachFromActor(detachRules);

	WeaponCollider->SetCollisionProfileName(TEXT("Weapons"));
	WeaponCollider->SetSimulatePhysics(true);
}
