// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "EndLineWithSound.generated.h"

UCLASS()
class CPP_PROJECT_API AEndLineWithSound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndLineWithSound();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	UShapeComponent* tBox;

	UPROPERTY(EditAnywhere)
		UMeshComponent* Goal;


	UPROPERTY(EditAnywhere)
		FVector BoxSize = FVector(1.5f, 1.5f, 1.5f);

	UPROPERTY(EditAnywhere)
		FVector GoalSize = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
		USoundBase* mSound;

	UFUNCTION()
		void triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
};
