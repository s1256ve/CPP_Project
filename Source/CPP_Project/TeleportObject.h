// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TeleportObject.generated.h"

UCLASS()
class CPP_PROJECT_API ATeleportObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	class UShapeComponent* tBox;

	UPROPERTY(EditAnywhere)
	class UMeshComponent* Mymesh;


	UPROPERTY(EditAnywhere)
		FVector BoxSize = FVector(1.5f, 1.5f, 1.5f);

	UPROPERTY(EditAnywhere)
		bool isMove = false;

	UPROPERTY(EditAnywhere)
		float MoveSpeed;

	UPROPERTY(EditAnywhere)
		bool isX = false;

	UPROPERTY(EditAnywhere)
		bool isY = false;

	UPROPERTY(EditAnywhere)
		bool isZ = false;

	UPROPERTY(EditAnywhere)
		bool isRotate = false;

	UPROPERTY(EditAnywhere)
		float RotateSpeed;

	UPROPERTY(EditAnywhere)
		bool RotatingX = false;

	UPROPERTY(EditAnywhere)
		bool RotatingY = false;

	UPROPERTY(EditAnywhere)
		bool RotatingZ = false;

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
		AActor* target;



	float RunningTime;

	UFUNCTION()
		void triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
};
