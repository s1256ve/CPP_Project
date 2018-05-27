// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "Item.h"
#include"Engine.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	tBox = CreateDefaultSubobject<UBoxComponent>(TEXT("tBox"));
	tBox->bGenerateOverlapEvents = true;
	tBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::triggerEnter);
	tBox->OnComponentEndOverlap.AddDynamic(this, &AItem::triggerExit);

	RootComponent = tBox;

	SM_Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Box"));
	SM_Box->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}
void AItem::Pickup()
{
	playerController->Inventory.Add(*ItemName);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You pick up "));
	Destroy();
}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (playerController != NULL)
	{
		if (playerController->isPickingup && bItemsWithInRange)
		{
			Pickup();
			
		}
	}

}

void AItem::getPlayer(AActor* player)
{
	playerController = Cast<ACPP_ProjectCharacter>(player);
}

void AItem::triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bItemsWithInRange = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Press E to pickup "), *ItemName));

	getPlayer(OtherActor);
}

void AItem::triggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bItemsWithInRange = false;
}
