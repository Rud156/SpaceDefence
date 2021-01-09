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
	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	StaticWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaitcWeaponMesh"));
	SkeletalWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalWeaponMesh"));
	ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingPoint"));
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));

	RootComponent = WeaponRoot;
	StaticWeaponMesh->SetupAttachment(WeaponRoot);
	SkeletalWeaponMesh->SetupAttachment(WeaponRoot);
	ShootingPoint->SetupAttachment(WeaponRoot);
	WeaponCollider->SetupAttachment(WeaponRoot);

	PrimaryActorTick.bCanEverTick = true;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::LoadRecoilData(FText recoilText)
{
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
			currentColumnIndex += 1;

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
	if (_currentRecoilTime > 0)
	{
		_currentRecoilTime -= DeltaTime;
	}

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

FVector2D ABaseWeapon::GetCurrentRecoilOffset()
{
	if (_currentRecoilIndex >= _recoilOffsets.Num())
	{
		_currentRecoilIndex = _recoilOffsets.Num() - 1;
	}

	FVector2D offset = _recoilOffsets[_currentRecoilIndex].offset;
	_currentRecoilIndex += 1;
	_currentRecoilTime = RecoilResetTime;

	return offset;
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