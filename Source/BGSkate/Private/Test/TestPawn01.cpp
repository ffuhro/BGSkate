// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestPawn01.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ATestPawn01::ATestPawn01()
{
	PrimaryActorTick.bCanEverTick = true;

	// Generate default subobjects
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBase"));
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetCollisionProfileName("Pawn");
	SetRootComponent(SphereComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);
	
}

void ATestPawn01::BeginPlay()
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

void ATestPawn01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestPawn01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent Success"));

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestPawn01::Move);
	}
}

void ATestPawn01::Move(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();
	if (CurrentValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Alive"));
	}
}

void ATestPawn01::Look(const FInputActionValue& Value)
{
	
}

