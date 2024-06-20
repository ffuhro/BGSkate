// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SkateWheeledVehiclePawn.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class BGSKATE_API ASkateWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	ASkateWheeledVehiclePawn();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


// ================================ COMPONENTS =====================================================================

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

// ================================ INPUTS =============================================================================
protected:

	// Enhanced Input Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* SteeringAction;


	float DriveCharge;
	
	// Functions

	void Move(const FInputActionValue& Value);
	void Steering(const FInputActionValue& Value);

	
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
