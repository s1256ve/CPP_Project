// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "Engine.h"
#include "UseWithKey.h"


// Sets default values
AUseWithKey::AUseWithKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	tBox = CreateDefaultSubobject<UBoxComponent>(TEXT("tBox"));
	tBox->bGenerateOverlapEvents = true;
	tBox->OnComponentBeginOverlap.AddDynamic(this,&AUseWithKey::triggerEnter);
	tBox->OnComponentEndOverlap.AddDynamic(this, &AUseWithKey::triggerExit);
	tBox->SetRelativeScale3D(BoxSize);

	RootComponent = tBox;

	SM_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Mesh"));
	SM_Mesh->SetupAttachment(RootComponent);

	PS_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_Effect"));
	PS_Effect->SetupAttachment(RootComponent);
	PS_Effect->SetVisibility(false);

	ResetTime = EffectTime;
}

// Called when the game starts or when spawned
void AUseWithKey::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(CountdownTimer,this,&AUseWithKey::AdvanceTime,1.0f,true);
	PS_Effect->SetVisibility(false);
}



// Called every frame
void AUseWithKey::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (playerController != NULL)
	{
		if (bPlayerIsInRange)
		{
			if (playerController->isInteracting && bPlayerIsInRange && playerController->Inventory.Contains("Matches") && !bActive)
			{
				Active();
			}
		}
	}
}

void AUseWithKey::GetPlayer(AActor* player)
{
	playerController = Cast<ACPP_ProjectCharacter>(player);
}

void AUseWithKey::triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GetPlayer(OtherActor);
	bPlayerIsInRange = true;

	if (playerController->Inventory.Contains("Matches"))
	{
		if(!bActive)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, HelpString);
	}
	else
	{
		if (!bActive)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString(TEXT("You must have key to active")));
	}
}


void AUseWithKey::triggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bPlayerIsInRange = false;
}

void AUseWithKey::Active()
{
	int32 XpGained = FMath::RandRange(10,100);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("You active now and Gain %d"),XpGained));
	playerController->Inventory.RemoveSingle("Match");
	bActive = true;
	PS_Effect->SetVisibility(true);
}

void AUseWithKey::AdvanceTime()
{
	if (bActive)
	{
		--ResetTime;

		if (ResetTime < 1)
		{
			TimerHasFinished();
		}
	}
}

void AUseWithKey::TimerHasFinished()
{
	PS_Effect->SetVisibility(false);
	bActive = false;
	ResetTime = EffectTime;
}