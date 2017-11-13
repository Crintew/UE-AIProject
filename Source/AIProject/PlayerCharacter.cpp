// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "AIProjectGameModeBase.h"
#include "AIProject.h"
#include "Engine.h"
//#include "KismetMathLibrary.generated.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PlayerPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PlayerPhysicsHandle"));
	m_PlayerPhysicsHandle->LinearDamping = 200.f;
	m_PlayerPhysicsHandle->LinearStiffness = 750.f;
	m_PlayerPhysicsHandle->AngularDamping = 500.f;
	m_PlayerPhysicsHandle->AngularStiffness = 1500.f;
	m_PlayerPhysicsHandle->InterpolationSpeed = 50.f;

	PlayerObjectHandle = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("PlayerObjectHandle"));

	PhysicsHandleDistance = 250.0;
	PhysicsTakeDistance = 400.0;
	bIsPhysicsHandleActive = false;

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
	/*
	if (bIsPhysicsHandleActive) {
		UpdatePhysicsHandle();
	}*/
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

AActor *APlayerCharacter::CheckGrabTrace(FVector &location, bool &pickup) {
	FHitResult hit_res(ForceInit);
	AActor *hit_res_actor = nullptr;;

	FCollisionQueryParams _TraceQueryParams;
	FCollisionResponseParams _TraceResponseParams;
	_TraceQueryParams.bTraceComplex = false;
	_TraceResponseParams.DefaultResponseParam;

	FVector trace_start = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
	FVector trace_end = trace_start + UKismetMathLibrary::Conv_RotatorToVector(UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation()).GetSafeNormal() * PhysicsTakeDistance * 10; //No magic here
	//FVector trace_end = trace_start + FRotationMatrix(UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation()).GetScaledAxis(EAxis::X) * PhysicsTakeDistance;
	
	pickup = false;
	location = trace_start;

	if (GetWorld()->LineTraceSingleByChannel(hit_res, trace_start, trace_end, ECollisionChannel::ECC_Visibility, _TraceQueryParams, _TraceResponseParams)) {
		hit_res_actor = hit_res.GetActor();
		if (IsValid(hit_res_actor)) {
			pickup = hit_res_actor->ActorHasTag(FName("Dummy"));
			location = hit_res.Location;
		}
	}
	return hit_res_actor;
}

void APlayerCharacter::GrabObject() {
	FHitResult hit_res(ForceInit);

	FCollisionObjectQueryParams _TraceObjectQueryParams = FCollisionObjectQueryParams::AllDynamicObjects;
	_TraceQueryParams.bTraceComplex = true;
	_TraceQueryParams.AddIgnoredComponent(GetCapsuleComponent());

	FVector trace_start = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
	FVector trace_end = trace_start + UKismetMathLibrary::GetForwardVector(UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation()) * PhysicsTakeDistance;
	//FVector trace_end = trace_start + FRotationMatrix(UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation()).GetScaledAxis(EAxis::X) * PhysicsTakeDistance;

	if (GetWorld()->LineTraceSingleByObjectType(hit_res, trace_start, trace_end, _TraceObjectQueryParams, _TraceQueryParams)) {
		//AddComponent(FName("None"), false, GetActorTransform(), PlayerPhysicsHandle);
		PlayerObjectHandle = hit_res.GetComponent();
		PlayerObjectHandle->InitializeComponent();
		m_PlayerPhysicsHandle->GrabComponent(PlayerObjectHandle, hit_res.BoneName, hit_res.Location, true);
		m_PlayerPhysicsHandle->SetTargetLocation(trace_end);
		//m_PlayerPhysicsHandle->GrabComponentAtLocation(PlayerObjectHandle, hit_res.BoneName, hit_res.Location);
		hit_res.Component.Get()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		AAIProjectGameModeBase* _Cast = Cast<AAIProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		_Cast->SetDummyFreeState(true);
	}
}

void APlayerCharacter::DropObject() {
	m_PlayerPhysicsHandle->ReleaseComponent();
	PlayerObjectHandle->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	AAIProjectGameModeBase* _Cast = Cast<AAIProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	_Cast->SetDummyFreeState(true);
}

void APlayerCharacter::UpdatePhysicsHandle() {
	PhysicsHandleLocation = (GetActorLocation() + (FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X) * PhysicsHandleDistance)) + FVector(0.0, 0.0, 50.0);
	m_PlayerPhysicsHandle->SetTargetLocation(PhysicsHandleLocation);
}
