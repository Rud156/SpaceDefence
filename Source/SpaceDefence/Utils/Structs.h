#pragma once


#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"

#include "Structs.generated.h"


class ATD_PlaceablesActors;


USTRUCT(BlueprintType)
struct FPlaceAbleData :public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
	TSubclassOf<ATD_PlaceablesActors> ActorRef;

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

	
	UStaticMesh*  StaticMeshRef =nullptr ;

};
