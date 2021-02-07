// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Containers/UnrealString.h"
#include "Misc/DefaultValueHelper.h"

ABaseWeapon::ABaseWeapon()
{
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
	StaticWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaitcWeaponMesh"));
	SkeletalWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalWeaponMesh"));
	ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingPoint"));

	RootComponent = WeaponCollider;
	StaticWeaponMesh->SetupAttachment(WeaponCollider);
	SkeletalWeaponMesh->SetupAttachment(WeaponCollider);
	ShootingPoint->SetupAttachment(WeaponCollider);

	HasRecoil = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasRecoil)
	{
		RecoilTick(DeltaTime);
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
		auto letter = recoilString[i];

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
				bool success = FDefaultValueHelper::ParseInt(currentNumberString, number);
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

	int centerRow = currentRowIndex / 2;
	int centerColumn = currentColumnIndex / 2;

	for (int j = 0; j < _recoilOffsets.Num(); j++)
	{
		auto recoilData = _recoilOffsets[j];

		int rowDiff = centerRow - recoilData.rowIndex;
		int columnDiff = centerColumn - recoilData.columnIndex;

		FVector2D offset = FVector2D(columnDiff * BASE_RECOIL_MULTIPLIER, rowDiff * BASE_RECOIL_MULTIPLIER);
		recoilData.offset = offset;

		_recoilOffsets[j] = recoilData;
	}
	_recoilOffsets.Sort(FSortRecoil());
}

bool ABaseWeapon::ShootTick(float DeltaTime)
{
	float currentTime = GetWorld()->GetTimeSeconds();
	float difference = currentTime - _lastShotTime;
	if (difference > FireRate)
	{
		_lastShotTime = currentTime;
		return true;
	}
	else
	{
		return false;
	}
}

void ABaseWeapon::RecoilTick(float deltaTime)
{
	if (_currentRecoilTime > 0)
	{
		_currentRecoilTime -= deltaTime;
		if (_currentRecoilTime <= 0)
		{
			_currentRecoilIndex = 0;
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
		recoilData.offset = FVector2D(_currentRandomIndex * BASE_RECOIL_MULTIPLIER, 0);

		if (_isLeft)
		{
			_currentRandomIndex -= 1;
			if (_currentRandomIndex <= -RandomXOffset)
			{
				_currentRandomShotCount += 1;

				if (_currentRandomShotCount > RandomXStopCount)
				{
					_isLeft = false;
					_currentRandomShotCount = 0;
				}
				else
				{
					float randomOffset = FMath::RandRange(-1.5f, 1.5f);
					recoilData.offset = FVector2D(randomOffset * BASE_RECOIL_MULTIPLIER, randomOffset * BASE_RECOIL_MULTIPLIER);
				}
			}
		}
		else
		{
			_currentRandomIndex += 1;
			if (_currentRandomIndex >= RandomXOffset)
			{
				_currentRandomShotCount += 1;

				if (_currentRandomShotCount > RandomXStopCount)
				{
					_isLeft = true;
					_currentRandomShotCount = 0;
				}
				else
				{
					float randomOffset = FMath::RandRange(-1.5f, 1.5f);
					recoilData.offset = FVector2D(randomOffset * BASE_RECOIL_MULTIPLIER, randomOffset * BASE_RECOIL_MULTIPLIER);
				}
			}
		}

		_currentRecoilIndex = _recoilOffsets.Num() - 1;
	}
	else
	{
		recoilData = _recoilOffsets[_currentRecoilIndex];
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
	FDetachmentTransformRules detachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true);
	DetachFromActor(detachRules);

	WeaponCollider->SetCollisionProfileName(TEXT("Weapons"));
	WeaponCollider->SetSimulatePhysics(true);
}