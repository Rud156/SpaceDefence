// Fill out your copyright notice in the Description page of Project Settings.


#include "MutantEnemy.h"

AMutantEnemy::AMutantEnemy() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMutantEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AMutantEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_currentAttackDuration > 0)
	{
		_currentAttackDuration -= DeltaTime;
		if (_currentAttackDuration <= 0)
		{
			_isAttacking = false;
		}
	}
}

void AMutantEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMutantEnemy::TeleportStart()
{

}

void AMutantEnemy::TeleportEnd()
{

}

void AMutantEnemy::Attack()
{
	_isAttacking = true;
	_currentAttackDuration = TempAttackDuration;
}

bool AMutantEnemy::GetIsAttacking()
{
	return _isAttacking;
}