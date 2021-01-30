// Fill out your copyright notice in the Description page of Project Settings.


#include "MutantEnemy.h"
#include "../Common/HealthAndDamageComp.h"

#include "Components/BoxComponent.h"

AMutantEnemy::AMutantEnemy() : Super()
{
	TeleportStartTime = 1.0f;
	TeleportEndTime = 1.5f;

	HandCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HandCollider"));
	HandCollider->SetupAttachment(GetMesh(), "WeaponHitBox");

	PrimaryActorTick.bCanEverTick = true;
}

void AMutantEnemy::BeginPlay()
{
	Super::BeginPlay();
	HandCollider->OnComponentBeginOverlap.AddDynamic(this, &AMutantEnemy::HandleHandCollisionEnter);
}

void AMutantEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_currentTime > 0)
	{
		_currentTime -= DeltaTime;

		switch (_teleportState)
		{
		case EMutantTeleportState::None:
			break;

		case EMutantTeleportState::TeleportStart:
			break;

		case EMutantTeleportState::TeleportEnd:
			SetMutantTeleportState(EMutantTeleportState::None);
			break;
		}
	}
}

int AMutantEnemy::TeleportStart()
{
	_currentTime = TeleportStartTime;
	SetMutantTeleportState(EMutantTeleportState::TeleportStart);
	OnTeleportStart();

	return _currentTime;
}

int AMutantEnemy::TeleportEnd()
{
	_currentTime = TeleportEndTime;
	SetMutantTeleportState(EMutantTeleportState::TeleportEnd);
	OnTeleportEnd();

	return _currentTime;
}

float AMutantEnemy::GetTeleportMoveDistance()
{
	return TeleportMoveDistance;
}

EMutantTeleportState AMutantEnemy::GetTeleportState()
{
	return _teleportState;
}

void AMutantEnemy::Attack()
{
	_currentAttackCount = FMath::RandRange(MinAttackCount, MaxAttackCount);
	_isAttacking = true;
}

void AMutantEnemy::AttackAnimationComplete(FString attackName)
{
	if (!_isAttacking)
	{
		return;
	}

	_currentAttackCount -= 1;
	if (_currentAttackCount <= 0)
	{
		_isAttacking = false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Attack Complete: " + attackName);
}

bool AMutantEnemy::GetIsAttacking()
{
	return _isAttacking;
}

int AMutantEnemy::GetAttackIndex()
{
	return _currentAttackCount;
}

void AMutantEnemy::SetMutantTeleportState(EMutantTeleportState teleportState)
{
	_teleportState = teleportState;
}

void AMutantEnemy::HandleHandCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!_isAttacking)
	{
		return;
	}

	UActorComponent* actorComponent = OtherActor->GetComponentByClass(UHealthAndDamageComp::StaticClass());
	UHealthAndDamageComp* healthAndDamage = Cast<UHealthAndDamageComp>(actorComponent);

	if (healthAndDamage != nullptr)
	{
		healthAndDamage->TakeDamage(DamageAmount);
	}
}