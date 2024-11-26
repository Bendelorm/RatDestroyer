// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RatDestroyer/Map/GridManager.h"
#include "GameFramework/Character.h"
#include "RatEnemy.generated.h"


class USphereComponent;

UCLASS()
class RATDESTROYER_API ARatEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARatEnemy();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> OverlapSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAlive;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float Health;
	
	UPROPERTY(VisibleAnywhere, Category = "RatEnemy")
	bool bIsMoving;

	UPROPERTY()
	AGridManager* GridManager;
	
	void AttackEnemy(float DamageTaken);

	void Death();

	void MoveTowardsNextCheckpoint();

	void startPath();

	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TSubclassOf<ARatEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 CurrentCheckpointIndex;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle MovementTimerHandle;
	FVector NextCheckpoint;
};
