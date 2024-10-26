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

	if(isAlive)
	{
		MoveToNextNode();
	}

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

void ARatEnemy::SpawnEnemy()
{
	this->isAlive = true;
	//Må add inn her om pathfinding
}

void ARatEnemy::MoveToNextNode()
{
	
}


