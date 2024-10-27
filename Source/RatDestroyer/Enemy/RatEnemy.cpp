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


// Called every frame put in code for pathfinding for enemy
void ARatEnemy::Tick(float DeltaTime)
 {
	Super::Tick(DeltaTime);


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

	if (Path.Num() == 0 || CurrentPathIndex >= Path.Num())
		return;

	FVector CurrentPosition = GetActorLocation();
	FVector TargetPosition = Path[CurrentPathIndex];


	//Move towards waypoint

	FVector Direction = (TargetPosition - CurrentPosition).GetSafeNormal();
	FVector NewPosition = CurrentPosition + Direction * MovementSpeed * DeltaTime;
	SetActorLocation(NewPosition);

		//If close enough to the target, move to the next point
	if (FVector::Dist(NewPosition, TargetPosition) < 10.f)
			{
				CurrentPathIndex;
			}
	

}






