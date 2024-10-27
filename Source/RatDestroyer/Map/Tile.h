// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Tile.generated.h"

class APlayerPawn;

UCLASS()
class RATDESTROYER_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	APlayerPawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* TileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Color")
	FColor DebugBoxColor;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialOne;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialTwo;

	UPROPERTY()
	bool bHasTower;

	UPROPERTY()
	bool bShowMat;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	//Functions

	UFUNCTION()
	void ChangeMatOnMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void EndMatOnMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void SetHasTower(bool bHasNewTower);

	UFUNCTION()
	bool GetHasTower();

	UStaticMeshComponent* GetStaticMesh() const;


	//Variables
	UPROPERTY(EditDefaultsOnly)
	bool bIsOccupied;
};
