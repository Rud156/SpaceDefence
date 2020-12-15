#pragma once


#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"

#include "Structs.generated.h"


class ATD_PlaceablesActors;


USTRUCT(BlueprintType)
struct FPlaceAbleData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadWrite)
	TSubclassOf<ATD_PlaceablesActors> ActorRef;


	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		FText Description;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
		float GoldCost;

	//add a upgradable stuffs here
	
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
	class UMaterialInterface* MaterialIcon;
	
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", BlueprintReadOnly)
	class UTexture2D* Icon;

	
	UStaticMesh*  StaticMeshRef =nullptr ;

};
