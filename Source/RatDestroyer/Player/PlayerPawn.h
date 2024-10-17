// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;
class UCameraComponent;
class UInputComponent;
class UInputAction;
class UInputMappingContext;

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


	//Variables
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	FVector2D CurrentInputMoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ScreenEdgePadding;

	UPROPERTY(EditDefaultsOnly)
	FVector2D MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	float ZoomSpeed;

	UPROPERTY(EditDefaultsOnly)
	bool ShouldRotate;

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed;



	//Input Mapping Context and Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ZoomAction;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;


	//Functions
	void MoveTriggered(const FInputActionValue& Value);

	void MoveCompleted();

	void Zoom(const FInputActionValue& Value);

	void RotationStarted();

	void RotationCompleted();

	void Look(const FInputActionValue& Value);

	void buildTower();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
