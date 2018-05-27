// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "CPP_ProjectHUD.generated.h"

UCLASS()
class ACPP_ProjectHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACPP_ProjectHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

