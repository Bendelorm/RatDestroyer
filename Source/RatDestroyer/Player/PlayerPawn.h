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
class ARDTowerActor;
class ATile;
class AGridManager;
class ARDTowerManager;

UCLASS()
class RATDESTROYER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	TObjectPtr<AGridManager> GridManager;

	TObjectPtr<ATile> SelectedTile;

	TObjectPtr<ARDTowerManager> TowerManager;

	TObjectPtr<ARDTowerActor> Tower;

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

	UPROPERTY(EditDefaultsOnly)
	bool bCanBuild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TSubclassOf<ARDTowerActor> BaseTower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Money")
	int32 Money;

	//Functions for making Player take Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Health")
	float Health;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* BuildModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* UndoTowerAction;

	//Functions

	UFUNCTION()
	void MoveTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void MoveCompleted();

	UFUNCTION()
	void Zoom(const FInputActionValue& Value);

	UFUNCTION()
	void RotationStarted();

	UFUNCTION()
	void RotationCompleted();

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Select(const FInputActionValue& Value);

	UFUNCTION()
	void BuildMode(const FInputActionValue& Value);

	UFUNCTION()
	void BuildTower(ATile* TargetTile);

	UFUNCTION()
	void UndoTower(const FInputActionValue& Value);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
