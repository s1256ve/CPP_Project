// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CPP_Project.h"
#include "CPP_ProjectCharacter.h"
#include "CPP_ProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "MySaveGame.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACPP_ProjectCharacter

ACPP_ProjectCharacter::ACPP_ProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	InitialStamina = 100.f;
	CurrentStamina = InitialStamina;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ACPP_ProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
}

void ACPP_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCurrentStamina(-DeltaTime * 0.01f * (InitialStamina));
}

float ACPP_ProjectCharacter::GetInitialStamina()
{
	return InitialStamina;
}

float ACPP_ProjectCharacter::GetCurrentStamina()
{
	return CurrentStamina;
}

void ACPP_ProjectCharacter::UpdateCurrentStamina(float Stamina)
{
	CurrentStamina = CurrentStamina +  Stamina;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACPP_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Save", IE_Pressed, this, &ACPP_ProjectCharacter::SaveGame);
	InputComponent->BindAction("Load", IE_Pressed, this, &ACPP_ProjectCharacter::LoadGame);

	InputComponent->BindAction("Interact",IE_Pressed,this, &ACPP_ProjectCharacter::BeginInteract);
	InputComponent->BindAction("Interact", IE_Released, this, &ACPP_ProjectCharacter::EndInteract);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ACPP_ProjectCharacter::BeginSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ACPP_ProjectCharacter::EndSprint);

	InputComponent->BindAction("Pickup", IE_Pressed, this, &ACPP_ProjectCharacter::BeginPickup);
	InputComponent->BindAction("Pickup", IE_Released, this, &ACPP_ProjectCharacter::EndPickup);
	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &ACPP_ProjectCharacter::ShowInventory);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACPP_ProjectCharacter::TouchStarted);
	if (EnableTouchscreenMovement(InputComponent) == false)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &ACPP_ProjectCharacter::OnFire);
	}

	InputComponent->BindAxis("MoveForward", this, &ACPP_ProjectCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACPP_ProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ACPP_ProjectCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ACPP_ProjectCharacter::LookUpAtRate);
}

void ACPP_ProjectCharacter::SaveGame()
{
	//Create an instance of savegame class
	UMySaveGame* SaveGameInstance =Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//Set savegame instance location equal to the player's current location
	SaveGameInstance->playerLocation = this->GetActorLocation();
	//Save the instance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("slot"), 0);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Save Game"));
}

void ACPP_ProjectCharacter::LoadGame()
{
	//Create an instance of savegame class
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//Load saved game into savegame instance variable
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("slot", 0));
	//Set player's location from savegame instance
	this->SetActorLocation(SaveGameInstance->playerLocation);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Load Game"));
}

void ACPP_ProjectCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<ACPP_ProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

}

void ACPP_ProjectCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ACPP_ProjectCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ACPP_ProjectCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ACPP_ProjectCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{

		if (bIsSprinting)
			Value *= 10;
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value/10);
	}
}

void ACPP_ProjectCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsSprinting)
			Value *= 10;
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value/10);
	}
}

void ACPP_ProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACPP_ProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ACPP_ProjectCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACPP_ProjectCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ACPP_ProjectCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ACPP_ProjectCharacter::TouchUpdate);
	}
	return bResult;
}

void ACPP_ProjectCharacter::BeginSprint()
{
	bIsSprinting = true;
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green, TEXT("Sprint Start"));
}

void ACPP_ProjectCharacter::EndSprint()
{
	bIsSprinting = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sprint End"));
}

void ACPP_ProjectCharacter::BeginPickup()
{
	isPickingup = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pickup Start"));
}

void ACPP_ProjectCharacter::EndPickup()
{
	isPickingup = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Pickup End"));
}

void ACPP_ProjectCharacter::ShowInventory()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Show Inventory"));

	for (auto&Item : Inventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item : %s "), *Item));
	}
}

void ACPP_ProjectCharacter::BeginInteract()
{
	isInteracting = true;
}

void ACPP_ProjectCharacter::EndInteract()
{
	isInteracting = false;
}