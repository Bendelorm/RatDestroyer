// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Tile.generated.h"

UCLASS()
class RATDESTROYER_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* BoxComponent;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
