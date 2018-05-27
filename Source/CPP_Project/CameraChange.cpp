// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "CameraChange.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACameraChange::ACameraChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));

	RootComponent = MyRoot;
}

// Called when the game starts or when spawned
void ACameraChange::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraChange::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	const float TimeBtwCameraChange = 2.0f;
	const float SmoothBlendTime = 0.75f;

	if (On)
	{
		CountDownToNextCamera -= DeltaTime;


		if (CountDownToNextCamera <= 0.0)
		{
			CountDownToNextCamera += TimeBtwCameraChange;

			APlayerController* ourPlayer = UGameplayStatics::GetPlayerController(this, 0);

			if (ourPlayer)
			{
				if (CameraTwo && (ourPlayer->GetViewTarget() == CameraOne))
				{
					ourPlayer->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
				}
				else if (CameraOne)
				{
					ourPlayer->SetViewTarget(CameraOne);
				}
			}
		}
	}
	

}

