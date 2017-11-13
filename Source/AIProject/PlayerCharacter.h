// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AIPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//UCharacterMovementComponent* CharacterMovement;

	//Is player controllable/can move
	bool bCanMove = false;
	bool bIsCrouching = false;
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPhysicsHandleActive;

	float PhysicsHandleDistance;
	float PhysicsTakeDistance;
	FVector PhysicsHandleLocation;

	FCollisionQueryParams _TraceQueryParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysicsHandleCategory)
	class UPhysicsHandleComponent *m_PlayerPhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPrimitiveComponent *PlayerObjectHandle;

	float MovementSpeedRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input for moving forward/backward
	UFUNCTION()
	void MoveForward(float Value);

	//Input for moving right/left
	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartCrouch();

	UFUNCTION()
	void StopCrouch();

	void StartRun();
	void StopRun();

	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Value);
	
	UFUNCTION(BlueprintCallable)
	AActor *CheckGrabTrace(FVector &location, bool &pickup);

	UFUNCTION(BlueprintCallable)
	void GrabObject();

	UFUNCTION(BlueprintCallable)
	void DropObject();

	UFUNCTION(BlueprintCallable)
	void UpdatePhysicsHandle();
};
