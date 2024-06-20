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
	SphereComponent->SetAngularDamping(0.001f);
	SphereComponent->SetLinearDamping(0.001f);
	SetRootComponent(SphereComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->SocketOffset.Z = 30.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->SetUsingAbsoluteRotation(true);
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	
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
	const FVector2d CurrentValue = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FVector Rotation = StaticMesh->GetForwardVector();
		Rotation.Z = 0.f;
		const FVector ForwardDirection = Rotation.GetSafeNormal();
		// SphereComponent->AddForce(ForwardDirection*5000*CurrentValue.X);
		SphereComponent->AddTorqueInDegrees(FVector(0,1000000,0));
	}
}

void ATestPawn01::Look(const FInputActionValue& Value)
{
	
}

