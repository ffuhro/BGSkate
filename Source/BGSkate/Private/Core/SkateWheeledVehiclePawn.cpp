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

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(GetRootComponent());
	
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

	CharacterAnimInstance = CharacterMesh->GetAnimInstance();
	
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
		DriveCharge -= DeltaSeconds * 0.3;
	}
	else
	{
		DriveCharge = 0.01f;
	}

	GetVehicleMovementComponent()->SetThrottleInput(DriveCharge);
	// UE_LOG(LogTemp, Display, TEXT("%f"), DriveCharge)
}

void ASkateWheeledVehiclePawn::Move()
{
	if (Controller != nullptr)
	{
		if (DriveCharge + .3f > 1)
		{
			DriveCharge = 1.f;
		}
		else
		{
			DriveCharge += .6f; // charge strength
		}
	}
	
}

void ASkateWheeledVehiclePawn::Braking(const FInputActionValue& Value)
{
	const float CurrentValue = Value.Get<float>();

	if (CurrentValue < -0.5)
	{
		DriveCharge = 0;
		GetVehicleMovementComponent()->SetThrottleInput(CurrentValue);
	
		UE_LOG(LogTemp, Display, TEXT("%f"), CurrentValue)

	}
}

void ASkateWheeledVehiclePawn::PushEnd()
{
	bCanPush = true;
}

void ASkateWheeledVehiclePawn::Steering(const FInputActionValue& Value)
{
	const float CurrentValue = Value.Get<float>();
	if (Controller != nullptr)
	{
		GetVehicleMovementComponent()->SetSteeringInput(CurrentValue);
	}
}

void ASkateWheeledVehiclePawn::PlayPushMontage()
{
	if (!PushMontage) { return; }

	if (bCanPush && CharacterAnimInstance->Montage_IsPlaying(PushMontage))
	{
		bCanPush = false;
		CharacterAnimInstance->Montage_Play(PushMontage);
		CharacterAnimInstance->Montage_JumpToSection(FName("2"));
		UE_LOG(LogTemp, Display, TEXT("Quick Push anim montage started"));
	}


	if (CharacterAnimInstance && !CharacterAnimInstance->Montage_IsPlaying(PushMontage))
	{
		bCanPush = false;
		CharacterAnimInstance->Montage_Play(PushMontage);
		CharacterAnimInstance->Montage_JumpToSection(FName("1"));
		UE_LOG(LogTemp, Display, TEXT("Push anim montage started"));
	}
	
}

void ASkateWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent Success"));

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::PlayPushMontage);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::Braking);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::Steering);
	}
}
