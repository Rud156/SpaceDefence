// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceDefenceHUD.generated.h"

UCLASS()
class ASpaceDefenceHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASpaceDefenceHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

