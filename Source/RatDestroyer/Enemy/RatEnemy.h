// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RatEnemy.generated.h"

class RDTreeNode;

UCLASS()
class RATDESTROYER_API ARatEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ARatEnemy();

	virtual void Tick(float deltatime) override;

	//UFUNCTION(Blueprintable)
	//void SetPath(const);

	UFUNCTION(BlueprintCallable)
	void StartMoving();


	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "RatEnemy")
	bool bIsmoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RatEnemy")
	float Damage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
