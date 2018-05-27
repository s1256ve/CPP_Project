// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "EndLineWithSound.h"


// Sets default values
AEndLineWithSound::AEndLineWithSound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	tBox = CreateDefaultSubobject<UBoxComponent>(TEXT("tBox"));

	RootComponent = tBox;

	tBox->bGenerateOverlapEvents = true;
	tBox->OnComponentBeginOverlap.AddDynamic(this, &AEndLineWithSound::triggerEnter);
	tBox->SetRelativeScale3D(BoxSize);

	Goal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal"));
	Goal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEndLineWithSound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndLineWithSound::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEndLineWithSound::triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		//UGameplayStatics::SpawnSoundAttached(mSound,OtherActor->GetRootComponent());
	}
    //D:\Unreal Projects\CPP_Project\Source\CPP_Project\EndLineWithSound.cpp(43) : warning C4996 : 'UGameplayStatics::PlaySoundAttached' : PlaySoundAttached has been renamed SpawnSoundAttached Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
	//D : \Epic Games\4.13\Engine\Source\Runtime\Engine\Classes\Kismet / GameplayStatics.h(463) : note: see declaration of 'UGameplayStatics::PlaySoundAttached'
}
