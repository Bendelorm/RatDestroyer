// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "BaseGizmos/GizmoElementShared.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 2000.0f;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//set initial zoom, location, rotation
	TargetLocation = GetActorLocation();
	TargetZoom = 3000.f;
	const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + -50, Rotation.Yaw, 0.0f);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraBounds();

	//moving the pawn in the correct direction
	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);

	//zooming the camera in the correct direction
	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent->TargetArmLength = InterpolatedZoom;

	//rotate the camera in the correct direction
	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	SpringArmComponent->SetRelativeRotation(InterpolatedRotation);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &APlayerPawn::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &APlayerPawn::Right);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &APlayerPawn::Zoom);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &APlayerPawn::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &APlayerPawn::RotateVertical);

	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed, this, &APlayerPawn::RotateRight);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed, this, &APlayerPawn::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Pressed, this, &APlayerPawn::EnableRotate);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Released, this, &APlayerPawn::DisableRotate);

}

void APlayerPawn::Forward(float AxisValue)
{
	if(AxisValue == 0.f)
	{
		return;
	}
	TargetLocation = SpringArmComponent->GetForwardVector() * AxisValue * MoveSpeed * TargetLocation;
}

void APlayerPawn::Right(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	TargetLocation = SpringArmComponent->GetRightVector() * AxisValue * MoveSpeed * TargetLocation;
}

void APlayerPawn::Zoom(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	const float Zoom = AxisValue * 100.0f;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, MinZoom, MaxZoom);
}

void APlayerPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, -45, 0.0f));
}

void APlayerPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, 45, 0.0f));
}

void APlayerPawn::EnableRotate()
{
	CanRotate = true;
}

void APlayerPawn::DisableRotate()
{
	CanRotate = false;
}

void APlayerPawn::RotateHorizontal(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, AxisValue, 0.f));
	}
}

void APlayerPawn::RotateVertical(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue, 0.f, 0.f));
	}
}

void APlayerPawn::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if(TargetRotation.Pitch < (RotatePitchMax * -1))
	{
		NewPitch = (RotatePitchMax * -1);
	}
	else if(TargetRotation.Pitch > (RotatePitchMin * -1))
	{
		NewPitch = (RotatePitchMin * -1);
	}

	//Set the new pitch and clamp any roll
	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.f);

	//Clamp desired location to within bounds
	TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.f);
}
