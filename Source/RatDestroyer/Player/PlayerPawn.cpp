// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "BaseGizmos/GizmoElementShared.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(RootComponent);

	ShouldRotate = false;
	MoveSpeed = FVector2D(500, 500);
	ScreenEdgePadding = FVector2D(50, 50);
	ZoomSpeed = 200;
	RotationSpeed = 50;

	SetActorRotation(FRotator::MakeFromEuler(FVector3d(0, -30, 0)));

}

void APlayerPawn::MoveTriggered(const FInputActionValue& Value)
{
	CurrentInputMoveSpeed = Value.Get<FVector2D>();
}

void APlayerPawn::MoveCompleted()
{
	CurrentInputMoveSpeed = FVector2D::Zero();
}

void APlayerPawn::Zoom(const FInputActionValue& Value)
{
	auto Location = GetActorLocation();
	Location.Z += Value.Get<float>() * ZoomSpeed * FApp::GetDeltaTime();
	SetActorLocation(Location);
}

void APlayerPawn::RotationStarted()
{
	ShouldRotate = true;
}

void APlayerPawn::RotationCompleted()
{
	ShouldRotate = false;
}

void APlayerPawn::Look(const FInputActionValue& Value)
{
	const auto& LookVector = Value.Get<FVector2D>();

	if (ShouldRotate)
	{
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Pitch += RotationSpeed * FApp::GetDeltaTime() * LookVector.Y;
		CurrentRotation.Yaw += RotationSpeed * FApp::GetDeltaTime() * LookVector.X;
		SetActorRotation(CurrentRotation);
	}
}

void APlayerPawn::buildTower()
{

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if(PlayerController = Cast<APlayerController>(Controller); IsValid(PlayerController))
	{
		PlayerController->SetShowMouseCursor(true);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}

}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Find the correct movement speed of the camera
	auto CurrentMoveSpeed = CurrentInputMoveSpeed;
	if (FVector2D MousePosition; CurrentInputMoveSpeed.X == 0 && CurrentInputMoveSpeed.Y == 0 && PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		int32 ViewportWidth, ViewportHeight;
		PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

		const auto X =
			-(MousePosition.X <= ScreenEdgePadding.X) | (MousePosition.X >= ViewportWidth - ScreenEdgePadding.X);

		const auto Y =
			(MousePosition.Y <= ScreenEdgePadding.Y) | -(MousePosition.Y >= ViewportHeight - ScreenEdgePadding.Y);

		CurrentMoveSpeed = FVector2D(X, Y);
	}

	//Move camera

	auto ForwardVector = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0);
	ForwardVector.Normalize();

	const auto Forward = ForwardVector * CurrentMoveSpeed.Y * MoveSpeed.Y * DeltaTime;
	const auto Sideways = GetActorRightVector() * CurrentMoveSpeed.X * MoveSpeed.X * DeltaTime;

	const auto NextLocation = GetActorLocation() + Forward + Sideways;
	SetActorLocation(NextLocation);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		//Move camera
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerPawn::MoveCompleted);

		//Zoom camera
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &APlayerPawn::Zoom);

		//Rotate camera
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Started, this, &APlayerPawn::RotationStarted);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Canceled, this, &APlayerPawn::RotationCompleted);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &APlayerPawn::RotationCompleted);

		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerPawn::Look);

	}
}
