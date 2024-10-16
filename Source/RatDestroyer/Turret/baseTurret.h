// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "baseTurret.generated.h"

UCLASS()
class RATDESTROYER_API AbaseTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AbaseTurret();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 baseDmg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 baseCost;

};
