// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "RandomItem.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ARandomItem::ARandomItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Mesh"));
	RootComponent = SM_Mesh;
}

// Called when the game starts or when spawned
void ARandomItem::BeginPlay()
{
	Super::BeginPlay();
	
	SM_Mesh->SetRelativeRotation(UKismetMathLibrary::RandomRotator());
}

// Called every frame
void ARandomItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}



