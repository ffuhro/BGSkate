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
	
protected:

	// ================================ VARIABLES ======================================================================

	UPROPERTY(VisibleAnywhere, Category=Animation)
	UAnimInstance* CharacterAnimInstance;
	
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	UAnimMontage* PushMontage;

	UPROPERTY(BlueprintReadWrite, Category=Animation)
	bool bCanPush = true;
	
	// ================================ COMPONENTS =====================================================================

	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category=Core)
	USkeletalMeshComponent* CharacterMesh;

	// ================================ INPUTS =========================================================================

	// Enhanced Input Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* BrakeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* SteeringAction;


	float DriveCharge;
	
	// ================================ INPUTS =========================================================================
	
	UFUNCTION(BlueprintCallable)
	void Move();

	void Braking(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void PushEnd();	
	
	void Steering(const FInputActionValue& Value);

	void PlayPushMontage();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
