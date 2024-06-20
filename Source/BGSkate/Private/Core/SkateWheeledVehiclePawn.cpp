// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SkateWheeledVehiclePawn.h"
// #include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASkateWheeledVehiclePawn::ASkateWheeledVehiclePawn()
{
	GetMesh()->SetSimulatePhysics(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->SocketOffset.Z = 30.f;
	SpringArm->TargetArmLength = 200.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);

	GetMesh()->SetMassOverrideInKg(NAME_None, 3.5f);
}

void ASkateWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup Inputs
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASkateWheeledVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DriveCharge - DeltaSeconds > 0.f)
	{
		DriveCharge -= DeltaSeconds * 0.5;
	}
	else
	{
		DriveCharge = 0.01f;
	}

	GetVehicleMovementComponent()->SetThrottleInput(DriveCharge);
	UE_LOG(LogTemp, Display, TEXT("%f"), DriveCharge)
}

void ASkateWheeledVehiclePawn::Move(const FInputActionValue& Value)
{
	const float CurrentValue = Value.Get<float>();
	if (Controller != nullptr)
	{
		if (DriveCharge + .3f > 1)
		{
			DriveCharge = 1.f;
		}
		else
		{
			DriveCharge += .3 * CurrentValue;
		}
	}
	
}
void ASkateWheeledVehiclePawn::Steering(const FInputActionValue& Value)
{
	const float CurrentValue = Value.Get<float>();
	if (Controller != nullptr)
	{
		GetVehicleMovementComponent()->SetSteeringInput(CurrentValue);
	}
	
}

void ASkateWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent Success"));

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::Move);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::Steering);
	}
}
