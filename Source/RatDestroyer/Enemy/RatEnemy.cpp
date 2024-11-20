// Fill out your copyright notice in the Description page of Project Settings.


#include "RatEnemy.h"

#include "Engine/DamageEvents.h"
#include "RatDestroyer/Map/GridManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Player/PlayerPawn.h"


ARatEnemy::ARatEnemy()
{
	
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;
	MovementSpeed = 300.0f;
	Health = 10;
	bIsMoving = false;
	isAlive = true;
	CurrentCheckpointIndex = 0;
	
	
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;


	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->RegisterWithPerceptionSystem();

	Tags.Add(FName("Enemy"));

}

// Called when the game starts or when spawned
void ARatEnemy::BeginPlay()
{ 
	Super::BeginPlay();


	UE_LOG(LogTemp, Log, TEXT("Rat Enemy Tag: %s"), *GetName());

	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	GridManager->NodeStart->WorldLocation;
	startPath();
}


void ARatEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

//Function to reduce RatEnemy Health  by the damage number and check if Rat Enemy is still alive
void ARatEnemy::AttackEnemy(float DamageTaken)
{
	UE_LOG(LogTemp, Log, TEXT("%p Shot"), this);
	this->Health -= DamageTaken;
	if(this->Health <= 0)
	{
		isAlive = false;
		Death();
	}
}

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
			PlayerPawn->Money = PlayerPawn->Money + 5;
		}
	}
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
			// When it has reached the End it Deals Damage to Player and Destorys itself
			APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if (PlayerPawn)
			{
				// Deal Damage to Player Health here 
				PlayerPawn->Health -= 10.0f;

				// Displays Health for our Player
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player Health: %.2f"), PlayerPawn->Health));
			}
			if (PlayerPawn->Health <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Player has died. Quitting game..."));
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

