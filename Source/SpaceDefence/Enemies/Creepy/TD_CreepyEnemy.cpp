// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_CreepyEnemy.h"
#include "DevelopmentTools/TD_DevelopmentTools.h"
#include "SpaceDefence/Public/Projectile/TD_BaseProjectile.h"
#include "Components/BoxComponent.h"
ATD_CreepyEnemy::ATD_CreepyEnemy()
{
	HandCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HandCollider"));
	HandCollider->SetupAttachment(GetMesh(), HandBoxCollisionLocation);
	HandCollider->SetGenerateOverlapEvents(true);
	HealthAndDamage->AddHealth(Health);

}

void ATD_CreepyEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HandCollider)
	{

		HandCollider->OnComponentBeginOverlap.AddDynamic(this, &ATD_CreepyEnemy::OnHandColliderOverlapBegin);

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

	if (CastState)
	{
		PrintToScreen_Color("Creepy Taking damage", FColor::Red);
		HealthAndDamage->TakeDamage(10);

	}
	//else
	//	PrintToScreen_1("Huh? %s", *OtherActor->GetName());
}
