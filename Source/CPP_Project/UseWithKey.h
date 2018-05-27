// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "CPP_ProjectCharacter.h"
#include "UseWithKey.generated.h"

UCLASS()
class CPP_PROJECT_API AUseWithKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUseWithKey();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UShapeComponent* tBox;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_Mesh;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* PS_Effect;
	
	ACPP_ProjectCharacter* playerController;

	UPROPERTY(EditAnywhere)
		FString HelpString = FString(TEXT("Press F to active"));

	UPROPERTY(EditAnywhere)
		int32 EffectTime = 10;
	
	int32 ResetTime;

	UPROPERTY(EditAnywhere)
		FVector BoxSize = FVector(1.5f, 1.5f, 1.5f);

	bool bPlayerIsInRange = false;

	bool bActive = false;

	void GetPlayer(AActor* player);

	void Active();

	void AdvanceTime();

	void TimerHasFinished();

	FTimerHandle CountdownTimer;

	UFUNCTION()
		void triggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void triggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
