// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "AIProject.h"
#include "Engine.h"
#include "KismetMathLibrary.generated.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PlayerPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PlayerPhysicsHandle"));
	//PlayerObjectHandle = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("PlayerObjectHandle"));

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	bIsCrouching = false;
	bIsRunning = false;
	MovementSpeedRate = 1.6;

	GetCharacterMovement()->MaxWalkSpeed = 800;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using PlayerCharacter."));
	}

	SetCanMove(true);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement binding
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//Camera control binding
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	//Camera control binding
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::StopCrouch);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRun);
}

//Input for moving forward/backward
void APlayerCharacter::MoveForward(float Value)
{
	if (bCanMove)
	{
		if (bIsRunning) {
			Value *= MovementSpeedRate;
		}
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, Value * 0.625);
	}
}

//Input for moving right/left
void APlayerCharacter::MoveRight(float Value)
{
	if (bCanMove)
	{
		if (bIsRunning) {
			Value *= MovementSpeedRate;
		}

		FVector Direction = GetActorRightVector();
		AddMovementInput(Direction, Value * 0.625);
	}
}

void APlayerCharacter::StartJump() {
	bPressedJump = true;
}

void APlayerCharacter::StopJump() {
	bPressedJump = false;
}

void APlayerCharacter::StartCrouch() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Crouch"));

	if (GetCharacterMovement()->IsMovingOnGround()) {
		GetCharacterMovement()->bWantsToCrouch = true;
		bIsCrouching = true;
	}
}

void APlayerCharacter::StopCrouch() {
	
	if (GetCharacterMovement()->IsMovingOnGround()) {
		GetCharacterMovement()->bWantsToCrouch = false;
		bIsCrouching = false;
	}
}

void APlayerCharacter::StartRun() {
	bIsRunning = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Running"));
}

void APlayerCharacter::StopRun() {
	bIsRunning = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Not running"));
}

void APlayerCharacter::SetCanMove(bool Value) {
	bCanMove = Value;
}
/*
void APlayerCharacter::GrabObject() {

}

void APlayerCharacter::DropObject() {

}

void APlayerCharacter::UpdatePhysicsHandle() {
	PhysicsHandleLocation = (GetActorLocation() + (FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X) * PhysicsHandleDistance)) + FVector(0.0, 0.0, 50.0);
	PlayerPhysicsHandle->SetTargetLocation(PhysicsHandleLocation);
}
*/