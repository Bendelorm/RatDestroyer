// Fill out your copyright notice in the Description page of Project Settings.


#include "RatEnemy.h"

#include "RatDestroyer/Map/GridManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Player/PlayerPawn.h"


ARatEnemy::ARatEnemy()
{
	
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	//Sets default options
	MovementSpeed = 300.0f;
	Health = 20;
	bIsMoving = false;
	isAlive = true;
	CurrentCheckpointIndex = 0;
	Tags.Add("Enemy");
	
	//Sets max walk speed for the movement of the Rat
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

// Called when the game starts or when spawned
void ARatEnemy::BeginPlay()
{ 
	Super::BeginPlay();

	//Access startNode location
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	GridManager->NodeStart->WorldLocation;

	//Begin to move along Path
	startPath();
}

//Called every frame
void ARatEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

//Function to reduce RatEnemy Health  by the damage number and check if Rat Enemy is still alive
void ARatEnemy::AttackEnemy(float DamageTaken)
{
	this->Health -= DamageTaken; //Decrease health
	if(this->Health <= 0)
	{
		//If health is below zero mark as not alive and Dead
		isAlive = false;
		Death();
	}
}

//Handel the death of Rato
void ARatEnemy::Death()
{

	if (isAlive)
	{
		Destroy();
	}
	if (!isAlive)
	{
		Destroy();
		//Give Player Money
		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (PlayerPawn)
		{
			PlayerPawn->Money = PlayerPawn->Money + 3; 
		}
	}
}

void ARatEnemy::startPath()
{
	//just makes sure that GridManager and checkpoints are valid
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

	//Marks the next checkpoint
	FVector TargetLocation = NextCheckpoint;
	TargetLocation.Z = 100.0f; 

	// Moving toward goal using interpolation
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), MovementSpeed);
	SetActorLocation(NewLocation);

	//Checks to see if next checkpoint has been reached
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
			// When it has reached the End it Deals Damage to Player and Destorys itself
			APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if (PlayerPawn)
			{
				// Deal Damage to Player Health here 
				PlayerPawn->Health -= 10.0f;
			}
			if (PlayerPawn->Health <= 0)
			{
				// quits the game for now probably gonna make an Lose screen
				UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
			}

			// Stop rat movement and destroy the rat
			bIsMoving = false;
			GetWorldTimerManager().ClearTimer(MovementTimerHandle);
			Death();
		}
	}
	
}

