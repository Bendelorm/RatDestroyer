// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RATDESTROYER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Components for player
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	UCameraComponent* CameraComponent;


	//Functions for camera control
	UFUNCTION()
	void Forward(float AxisValue);

	UFUNCTION()
	void Right(float AxisValue);

	UFUNCTION()
	void Zoom(float AxisValue);

	UFUNCTION()
	void RotateRight();

	UFUNCTION()
	void RotateLeft();

	UFUNCTION()
	void EnableRotate();

	UFUNCTION()
	void DisableRotate();

	UFUNCTION()
	void RotateHorizontal(float AxisValue);

	UFUNCTION()
	void RotateVertical(float AxisValue);

	UFUNCTION()
	void CameraBounds();


	//Player controlled camera settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float MoveSpeed = 20.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float RotateSpeed = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float RotatePitchMin = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float RotatePitchMax = 80.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float ZoomSpeed = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float MinZoom = 500.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
	float MaxZoom = 4000.0f;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY()
	float TargetZoom;

	UPROPERTY()
	bool CanRotate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
