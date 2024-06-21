// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SkateWheeledVehiclePawn.h"
// #include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ASkateWheeledVehiclePawn::ASkateWheeledVehiclePawn()
{
	GetMesh()->SetSimulatePhysics(true);
	
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetCollisionProfileName(FName("CharacterMesh"));
	CharacterMesh->SetupAttachment(GetRootComponent());
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArm->SocketOffset.Z = 30.f;
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);

	GetMesh()->SetMassOverrideInKg(NAME_None, 3.5f);
}

void ASkateWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	CharacterAnimInstance = CharacterMesh->GetAnimInstance();

	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	
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
	UpdateFallingState(DeltaSeconds);
	UpdateAirborneBehavior(DeltaSeconds);
}

// -====================== Base movement ============================-

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
	
		// UE_LOG(LogTemp, Display, TEXT("%f"), CurrentValue)

	}
}

// -=========================== Push ==================================-

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

// -============================= Jump Area ==================================-

void ASkateWheeledVehiclePawn::PlayJumpMontage()
{
	if (!JumpMontage) { return; }

	if (bCanJump)
	{
		bCanJump = false;
		CharacterAnimInstance->Montage_Play(JumpMontage);
		UE_LOG(LogTemp, Display, TEXT("Jump  anim montage started"));

	}	
}

void ASkateWheeledVehiclePawn::Jump()
{
	GetMesh()->SetAllPhysicsLinearVelocity(FVector(0.f, 0.f, 500), true);
	UE_LOG(LogTemp, Display, TEXT("Impulse added"));
	// GetWorldTimerManager().SetTimer(JumpEndHandle, this, &ASkateWheeledVehiclePawn::JumpStop, 3.f);

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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::PlayJumpMontage);
		EnhancedInputComponent->BindAction(AirControlAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::AirControl);
		EnhancedInputComponent->BindAction(CameraControlAction, ETriggerEvent::Triggered, this, &ASkateWheeledVehiclePawn::CameraControl);
	}
}

void ASkateWheeledVehiclePawn::CameraControl(const FInputActionValue& Value)
{
	const FVector2d CurrentValue = Value.Get<FVector2d>();
	//
	AddControllerYawInput(CurrentValue.X);
	AddControllerPitchInput(CurrentValue.Y);

	UE_LOG(LogTemp, Warning, TEXT("X: %f"), CurrentValue.X);
	UE_LOG(LogTemp, Warning, TEXT("Y: %f"), CurrentValue.Y);

}

void ASkateWheeledVehiclePawn::AirControl(const FInputActionValue& Value)
{
	if (!MovementStatus == EMS_Falling) return;
	FVector2d CurrentValue = Value.Get<FVector2d>();


	GetMesh()->AddTorqueInDegrees(GetMesh()->GetUpVector() * 500.f * CurrentValue.X, NAME_None, true);
	UE_LOG(LogTemp, Warning, TEXT("Y: %f"), CurrentValue.X);

}

// -================================= FALLING STATE ============================------------

void ASkateWheeledVehiclePawn::UpdateFallingState(const float DeltaSeconds)
{
	// Handle Falling

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingleByProfile(this, GetActorLocation(), GetActorLocation() + GetActorUpVector() * - 10.f ,FVector(20.f, 7.f, 4.f), GetActorRotation(), FName("IgnoreOnlyPawn"), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);

	if (!HitResult.bBlockingHit && MovementStatus == EMS_Riding)
	{
		MovementStatus = EMS_Falling;
		bCanJump = false;

		GetMesh()->SetAngularDamping(2.f);
		
		
		GetVehicleMovementComponent()->CenterOfMassOverride = FVector(0.f, 0.f, 50.f);

		GetVehicleMovementComponent()->TorqueControl.Enabled = false;
		GetVehicleMovementComponent()->TargetRotationControl.Enabled = false;
		GetVehicleMovementComponent()->StabilizeControl.Enabled = false;

		DriveCharge = 0.f;
		
		UE_LOG(LogTemp, Warning, TEXT("Player Is Falling"));
	}
	else if (HitResult.bBlockingHit && MovementStatus == EMS_Falling)
	{
		MovementStatus = EMS_Riding;
		bCanJump = true;

		GetMesh()->SetAngularDamping(.01f);		
		
		GetVehicleMovementComponent()->CenterOfMassOverride = FVector(0.f, 0.f, -5.f);

		GetVehicleMovementComponent()->TorqueControl.Enabled = false;
		GetVehicleMovementComponent()->TargetRotationControl.Enabled = false;
		GetVehicleMovementComponent()->StabilizeControl.Enabled = false;
		

		UE_LOG(LogTemp, Warning, TEXT("Player is Riding"));
	}


	// UE_LOG(LogTemp, Warning, TEXT("Center Of Mass is: %s"), *GetVehicleMovementComponent()->CenterOfMassOverride.ToString());
}

void ASkateWheeledVehiclePawn::UpdateAirborneBehavior(const float DeltaSeconds)
{

	if (!MovementStatus == EMS_Falling) return;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	FHitResult HitResultSlope;
	UKismetSystemLibrary::LineTraceSingleByProfile(this, GetActorLocation(), GetActorLocation() + FVector(0.f, 0.f, - 200.f) ,FName("IgnoreOnlyPawn"), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResultSlope, true);

	if (HitResultSlope.bBlockingHit)
	{
		const FRotator RotationTarget = FRotator(HitResultSlope.Normal.ForwardVector.Rotation().Pitch, GetActorRotation().Yaw, HitResultSlope.Normal.ForwardVector.Rotation().Roll);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotationTarget, DeltaSeconds, 1), ETeleportType::TeleportPhysics);
		UE_LOG(LogTemp, Warning, TEXT("Interping rotation to surface"));
	}
	// GetMesh()->AddTorqueInDegrees(CameraManager->GetActorUpVector() * 3000000.f);
}
