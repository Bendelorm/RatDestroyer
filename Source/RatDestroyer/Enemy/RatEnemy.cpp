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
	MoveAlongPath(); // Call your movement logic here
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


void ARatEnemy::MoveAlongPath()
{
    float AcceptableDistance = 50.f;

    // Check if the path is empty
    if (Path.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Path is empty, cannot move."));
        return; // Avoid processing if there's no path
    }

    // Check if CurrentPathIndex is out of bounds
    if (CurrentPathIndex >= Path.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentPathIndex is out of bounds!"));
        return; // Prevent accessing out of bounds
    }

    FVector TargetLocation = Path[CurrentPathIndex];

    // Move towards the target location
    // Simple linear interpolation for movement
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

    // Move the enemy towards the target location
    SetActorLocation(CurrentLocation + Direction * MovementSpeed * GetWorld()->DeltaTimeSeconds); // MoveSpeed should be defined in your class

    // Check if reached the target location
    if (FVector::Dist(CurrentLocation, TargetLocation) < AcceptableDistance) // Check if close enough to the target
    {
        CurrentPathIndex++; // Move to the next target

        // Check if reached the end of the path
        if (CurrentPathIndex >= Path.Num())
        {
            UE_LOG(LogTemp, Warning, TEXT("Reached the end of the path."));
            Destroy(); // Destroy or handle the enemy reaching the goal
        }
    }
}




