// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "Components/HomingTargetComponent.h"
#include "RatDestroyer/Enemy/RatEnemy.h"
#include "Projectile.generated.h"

UCLASS()
class RATDESTROYER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	/**The collision shape of the bullet*/
	UPROPERTY(EditAnywhere, Category = "Bullet")
	USphereComponent* CollisionSphere{ nullptr };

	// Mesh bullet
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UStaticMeshComponent* MeshComponent{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	TSubclassOf<ARatEnemy> Enemy;


	/**Time before bullet selfdestruct*/
	UPROPERTY(EditAnywhere, Category = "Bullet")
	float TimeBeforeDestroy = 2.f ;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float baseDamage = 30.0f;

	//functions 

	UFUNCTION() 
	void TargetEnemy();

	UFUNCTION() 
	void ProjectileDestroy(); 


	//variables




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
