// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WheeledVehiclePawn.h"
#include "SkateWheeledVehiclePawn.generated.h"

class UPhysicalAnimationComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */


UENUM()
enum EMovementStatus
{
	EMS_Riding,
	EMS_Falling
};

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
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	UAnimMontage* JumpMontage;

	UPROPERTY(BlueprintReadWrite, Category=Animation)
	bool bCanPush = true;
	UPROPERTY(BlueprintReadWrite, Category=Animation)
	bool bCanJump = true;
	
	float DriveCharge;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category=Core)
	USkeletalMeshComponent* CharacterMesh;

	FTimerHandle JumpEndHandle;

	UPROPERTY(VisibleAnywhere, Category="Core|Camera")
	APlayerCameraManager* CameraManager;
	
	// ================================ INPUTS =========================================================================

	// Enhanced Input Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputAction* BrakeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputAction* SteeringAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputAction* AirControlAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ActionInput)
	UInputAction* CameraControlAction;
	
	// ================================ FUNCTIONS ======================================================================
	
	UFUNCTION(BlueprintCallable)
	void Move();

	void Braking(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void PushEnd();	
	
	void Steering(const FInputActionValue& Value);

	void PlayPushMontage();

	void PlayJumpMontage();

	UFUNCTION(BlueprintCallable)
	void Jump();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void CameraControl(const FInputActionValue& Value);
	
	void AirControl(const FInputActionValue& Value);

	// ================================ STATES UPDATES =================================================================

	EMovementStatus MovementStatus = EMS_Falling;
	void UpdateFallingState(const float DeltaSeconds);
	void UpdateAirborneBehavior(const float DeltaSeconds);
};
