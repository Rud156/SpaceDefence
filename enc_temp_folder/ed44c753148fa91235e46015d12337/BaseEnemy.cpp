// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/CapsuleComponent.h"

#include "DevelopmentTools/TD_DevelopmentTools.h"
#include "Kismet/GameplayStatics.h"

void ABaseEnemy::PlayRandomSoundWhenMoving()
{
	//this->GetVelocity().len
	//check if its moving and active
	{
		if(Settings.RandomSoundToPlayWhenMoving)
			UGameplayStatics::PlaySoundAtLocation(this, Settings.RandomSoundToPlayWhenMoving, this->GetActorLocation());
	}
	
	float InitalTime = FMath::RandRange(5.0f, 20.f);
	//PrintToScreen_1("Next sound is playing in %f",InitalTime);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseEnemy::PlayRandomSoundWhenMoving, InitalTime, false);
	
}

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//Optimization
	//Collision
	
	GetCapsuleComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	GetMesh()->bDisableClothSimulation = false;

	//sets the simulation generates hit events for the mesh
	GetMesh()->BodyInstance.bNotifyRigidBodyCollision = true;
	GetMesh()->SetCollisionProfileName("Enemies");
	GetMesh()->SetCanEverAffectNavigation(bCanAffectNavigation);

	//Lighting
	GetMesh()->CastShadow = bCastShadow;

	//Navigation
	GetMesh()->bFillCollisionUnderneathForNavmesh = false;

	//Block input here.
	bBlockInput = true;
	AutoReceiveInput = EAutoReceiveInput::Disabled;
	
	
}

void ABaseEnemy::BeginPlay()
{
	
	Super::BeginPlay();
	float InitalTime = FMath::RandRange(1.0f, 10.0f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseEnemy::PlayRandomSoundWhenMoving, InitalTime, false);
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::NotifyEnemyDied()
{
	OnEnemyDied.Broadcast();
}

float ABaseEnemy::GetCurrentHealth_Implementation()
{
	return  _currentHealth;
}

float ABaseEnemy::GetMaxHealth_Implementation()
{
	return  MaxHealth;
}

void ABaseEnemy::SetCurrentHealth_Implementation(float HealthAmount)
{
	_currentHealth = HealthAmount;
	if (_currentHealth > MaxHealth)
	{
		_currentHealth = MaxHealth;
	}
}

void ABaseEnemy::TakeDamage_Implementation(float DamageAmount)
{
	_currentHealth -= DamageAmount;
	if (_currentHealth <= 0)
	{
		NotifyEnemyDied();
	}
}
