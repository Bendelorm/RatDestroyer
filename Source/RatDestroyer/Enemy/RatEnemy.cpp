// Fill out your copyright notice in the Description page of Project Settings.


#include "RatEnemy.h"
#include "RatDestroyer/Map/GridManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARatEnemy::ARatEnemy()
{
	
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;
	MovementSpeed = 300.0f;
	Health = 10;
	bIsMoving = false;
	CurrentCheckpointIndex = 0;
	
	
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

// Called when the game starts or when spawned
void ARatEnemy::BeginPlay()
{ 
	Super::BeginPlay();

	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	GridManager->NodeStart->WorldLocation;
	startPath();
}


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
	return isAlive;
}

void ARatEnemy::startPath()
{
	if (!GridManager || GridManager->VisitedCheckpoints.Num() == 0)
	{
		return;
	}

	CurrentCheckpointIndex = 0;
	NextCheckpoint = GridManager->VisitedCheckpoints[CurrentCheckpointIndex];
	NextCheckpoint.Z = 50.0f; // Set the initial target height

	bIsMoving = true;

	// Set up a timer to call MoveTowardsNextCheckpoint regularly for smooth movement
	float MoveInterval = 0.02f; // Controls how often MoveTowardsNextCheckpoint updates
	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ARatEnemy::MoveTowardsNextCheckpoint, MoveInterval, true);
}


void ARatEnemy::MoveTowardsNextCheckpoint()
{
	// Checks for the checkpoints and allows movement
	if (!bIsMoving || !GridManager || GridManager->PathCheckpoints.Num() == 0)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();

	
	FVector TargetLocation = NextCheckpoint;
	TargetLocation.Z = 100.0f; 

	// Moving toward goal using interpolation
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), MovementSpeed);
	SetActorLocation(NewLocation);

	
	float DistanceToCheckpoint = FVector::Dist(CurrentLocation, TargetLocation);
	if (DistanceToCheckpoint <= 10.0f) 
	{
		CurrentCheckpointIndex++;

		// Check if there are more checkpoints
		if (CurrentCheckpointIndex < GridManager->PathCheckpoints.Num())
		{
			NextCheckpoint = GridManager->PathCheckpoints[CurrentCheckpointIndex];
			NextCheckpoint.Z = 100.0f; 
		}
		else
		{
			// Reached the final checkpoint, stop movement
			bIsMoving = false;
			GetWorldTimerManager().ClearTimer(MovementTimerHandle);
		}
	}
}

