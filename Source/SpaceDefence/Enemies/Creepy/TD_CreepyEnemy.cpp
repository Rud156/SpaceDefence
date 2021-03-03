// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_CreepyEnemy.h"
#include "DevelopmentTools/TD_DevelopmentTools.h"
#include "SpaceDefence/Public/Projectile/TD_BaseProjectile.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

ATD_CreepyEnemy::ATD_CreepyEnemy() : Super()
{
	HandCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HandCollider"));
	HandCollider->SetupAttachment(GetMesh(), HandBoxCollisionLocation);
	HandCollider->SetGenerateOverlapEvents(true);
}

void ATD_CreepyEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HandCollider)
	{

		HandCollider->OnComponentBeginOverlap.AddDynamic(this, &ATD_CreepyEnemy::OnHandColliderOverlapBegin);
		HealthAndDamage->OnUnitDied.AddDynamic(this, &ATD_CreepyEnemy::Death);

	}
	if (HeadColliderComponent)
	{
		HeadColliderComponent->OnComponentHit.AddDynamic(this, &ATD_CreepyEnemy::OnCompHitHead);
	}
	GetMesh()->OnComponentHit.AddDynamic(this, &ATD_CreepyEnemy::OnCompHit);
}

void ATD_CreepyEnemy::OnHandColliderOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto CastState = Cast<ATD_PlaceablesActors>(OtherActor);
	if (CastState)
	{
		//PrintToScreen_Color("Hit Hit ", FColor::Red);
		CastState->TakeDamage(DamageAmount);

	}
	//else
	//	PrintToScreen_1("Huh? %s", *OtherActor->GetName());
}

void ATD_CreepyEnemy::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	auto CastState = Cast<ATD_BaseProjectile>(OtherActor);

	if (CastState && bIsAlive)
	{
		const float damageAmount = CastState->DamageAmount;
		HealthAndDamage->TakeDamage(damageAmount);

	}
	//else
	//	PrintToScreen_1("Huh? %s", *OtherActor->GetName());
}

void ATD_CreepyEnemy::OnCompHitHead(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	auto CastState = Cast<ATD_BaseProjectile>(OtherActor);

	if (CastState && bIsAlive)
	{
		const float damageAmount = CastState->DamageAmount;
		HealthAndDamage->TakeDamage(damageAmount * HeadShotMultiplier);
	}
}

void ATD_CreepyEnemy::Death(AActor* Actor)
{
	CurrentAnimationState = ECreepyAnimState::Dead;
	bIsAlive = false;
	CurrencyManagerRef->AddCurrency(AmountToGive);
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ATD_CreepyEnemy::RemoveDeadBody, DeadBodyTimer, false, -1);
}

void ATD_CreepyEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATD_CreepyEnemy, CurrentAnimationState)
}

