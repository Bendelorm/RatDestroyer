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
	if (GridManager->VisitedCheckpoints.Num() == 0)
	{
		return;
	}
	//FVector CurrentCheckpoint = GridManager->VisitedCheckpoints
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
	GridManager->VisitedCheckpoints[0];
}






