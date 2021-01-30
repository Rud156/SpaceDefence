#pragma once


#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"

#include "Structs.generated.h"


class ATD_PlaceablesActors;
class ABaseEnemy;
class ATD_SpawnPoints;

USTRUCT(BlueprintType)
struct FPlaceAbleData :public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
		TSubclassOf<ATD_PlaceablesActors> ActorRef;

	// Temporary static mesh, add only if you have a different static meshes in the actor
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
		UStaticMesh* TempStaticMesh;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		USoundCue* PlacementSound;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		USoundCue* DamageSound;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		USoundCue* DestructionSound;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		USoundCue* UpgradeSound;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		FText Description;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		float GoldCost;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		float Health;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		bool bCanBeUpgraded;

	//add a upgradable stuffs here

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		class UMaterialInterface* MaterialIcon;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		class UTexture2D* Icon;


	UStaticMesh* StaticMeshRef = nullptr;

};

USTRUCT(BlueprintType)
struct FEnemySettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Enemy Settings", BlueprintReadOnly)
		USoundCue* RandomSoundToPlayWhenMoving;
	
	UPROPERTY(EditAnywhere, Category = "Enemy Settings", BlueprintReadOnly)
		USoundCue* AttackSound;
	
	UPROPERTY(EditAnywhere, Category = "Enemy Settings", BlueprintReadOnly)
		USoundCue* TakeDamageSound;

	UPROPERTY(EditAnywhere, Category = "Enemy Settings", BlueprintReadOnly)
		USoundCue* AttackDamageSound;

	UPROPERTY(EditAnywhere, Category = "Enemy Settings", BlueprintReadOnly)
		USoundCue* DeathSound;
};
USTRUCT(BlueprintType)
struct FWaveGroup
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Group Settings", BlueprintReadWrite)
		TSubclassOf<ABaseEnemy> EnemyType;
	UPROPERTY(EditAnywhere, Category = "Group Settings", BlueprintReadWrite)
		int EnemyCount;
	UPROPERTY(EditAnywhere, Category = "Group Settings", BlueprintReadWrite)
		float EnemyInterval;
	UPROPERTY(EditAnywhere, Category = "Group Settings", BlueprintReadWrite)
		float SpawnRadius = 50;
	UPROPERTY(EditAnywhere, Category = "Group Settings", BlueprintReadWrite)
		ATD_SpawnPoints* OptionalSpawnLocation = nullptr;
};

USTRUCT(BlueprintType)
struct FWaveSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Wave Settings", BlueprintReadWrite)
		ATD_SpawnPoints* SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Wave Settings", BlueprintReadWrite)
		float GroupInterval;
	UPROPERTY(EditAnywhere, Category = "Wave Settings", BlueprintReadWrite)
		TArray<FWaveGroup> GroupSettings;



	FWaveSettings(ATD_SpawnPoints* I_SpawnPoints) :SpawnPoints(I_SpawnPoints), GroupInterval(0.0f)
	{

	}
	FWaveSettings()
	{}
};

USTRUCT(BlueprintType)
struct FRecoilOffset
{
	GENERATED_BODY();

	int index;
	int rowIndex;
	int columnIndex;
	FVector2D offset;
};
