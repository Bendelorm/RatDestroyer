// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ratdestroyer/Map/GridManager.h"
#include "RatEnemy.generated.h"


class USphereComponent;

UCLASS()
class RATDESTROYER_API ARatEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ARatEnemy();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> OverlapSphere;
	
	

protected:
	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float Health;
	
	UPROPERTY(VisibleAnywhere, Category = "RatEnemy")
	bool bIsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAlive;

	UPROPERTY()
	AGridManager* GridManager;

public:
	virtual bool AttackEnemy(float DamageTaken);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TSubclassOf<ARatEnemy> EnemyClass;

	//Functions

	void startPath();

	private:
		

	
};
