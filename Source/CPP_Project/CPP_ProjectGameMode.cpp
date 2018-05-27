// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CPP_Project.h"
#include "CPP_ProjectGameMode.h"
#include "CPP_ProjectHUD.h"
#include "CPP_ProjectCharacter.h"
#include "Blueprint/UserWidget.h"

void ACPP_ProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	ACPP_ProjectCharacter* myCharacter = Cast<ACPP_ProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (PlayerHUDClass!=nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();	
		}
	}
}

ACPP_ProjectGameMode::ACPP_ProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACPP_ProjectHUD::StaticClass();
}
