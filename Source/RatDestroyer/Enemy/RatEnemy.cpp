// Fill out your copyright notice in the Description page of Project Settings.


#include "RatEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"



ARatEnemy::ARatEnemy()
{
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;
	MovementSpeed = 300.0f;
	Health = 10;
	bIsMoving = false;
	
	
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

// Called when the game starts or when spawned
void ARatEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void ARatEnemy::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	
//}

void ARatEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveAlongPath(DeltaTime); // Call your movement logic here
}



//Function to reduce RatEnemy Health  by the damage number and check if Rat Enemy is still alive
bool ARatEnemy::AttackEnemy(float DamageTaken)
{
	this->Health -= DamageTaken;
	
	if(this->Health <= 0)
	{
		isAlive = false;
	}
	//Returns the status of enemy (true if its Alive and False if not)
	return isAlive;
}


void ARatEnemy::SetPath(const TArray<FVector>& NewPath)
{
	
	Path = NewPath;
	CurrentPathIndex = 0;
}


void ARatEnemy::MoveAlongPath(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	if (Path.Num() > 0)
	{
		FVector NextWaypoint = Path[0];
		FVector Direction = (NextWaypoint - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + Direction * MovementSpeed * DeltaTime;
		const float AcceptableDistance = 20.0f;

		SetActorLocation(NewLocation);

		// Check if we've reached the next waypoint
		if (FVector::Dist(NewLocation, NextWaypoint) < AcceptableDistance)
		{
			Path.RemoveAt(0); // Remove the reached waypoint
		}
	}

}






