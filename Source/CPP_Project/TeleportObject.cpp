// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "TeleportObject.h"


// Sets default values
ATeleportObject::ATeleportObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	tBox = CreateDefaultSubobject<UBoxComponent>(TEXT("tBox"));
	tBox->bGenerateOverlapEvents = true;
	tBox->OnComponentBeginOverlap.AddDynamic(this, &ATeleportObject::triggerEnter);
	tBox->SetRelativeScale3D(BoxSize);
	RootComponent = tBox;
	Mymesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mymesh"));
	Mymesh->SetupAttachment(RootComponent);
	MoveSpeed = 0.0f;
	RotateSpeed = 0.0f;
}

// Called when the game starts or when spawned
void ATeleportObject::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerLocation = target->GetActorLocation();
}

// Called every frame
void ATeleportObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	if (isMove)
	{
		if (isX)
			NewLocation.X += DeltaHeight*MoveSpeed;
		if (isY)
			NewLocation.Y += DeltaHeight*MoveSpeed;
		if (isZ)
			NewLocation.Z += DeltaHeight*MoveSpeed;
	}
	if (isRotate)
	{
		if (RotatingX)
			NewRotation.Roll += DeltaHeight*RotateSpeed;
		if (RotatingY)
			NewRotation.Pitch += DeltaHeight*RotateSpeed;
		if (RotatingZ)
			NewRotation.Yaw += DeltaHeight*RotateSpeed;
	}


	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
}

void ATeleportObject::triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
		OtherActor->SetActorLocation(PlayerLocation);

}
