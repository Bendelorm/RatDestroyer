// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "RDTowerActor.generated.h"


class ARatEnemy;
class USphereComponent;
class ATile;

UCLASS()
class RATDESTROYER_API ARDTowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ARDTowerActor();

	TObjectPtr<ATile> Tile;

	TArray<AActor*> DetectedEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* TowerMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UBoxComponent* TargetRadius;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AProjectile> Projectile;




	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseAccuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackTime;

	


	//Functions

	UFUNCTION()
	void OnTargetDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetLost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
