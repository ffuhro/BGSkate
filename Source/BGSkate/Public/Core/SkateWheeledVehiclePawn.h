// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION(BlueprintCallable)
	void JumpStop();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
