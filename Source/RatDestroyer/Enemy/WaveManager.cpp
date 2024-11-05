// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "RatEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Map/GridManager.h"




// Sets default values
AWaveManager::AWaveManager(): GridManager(nullptr), EnemySpawned(0), bActiveWave(false)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    GridManager->NodeStart->WorldLocation;
}

    void AWaveManager::StartWave() 
    {
    if (!GridManager || !GridManager->NodeStart || !GridManager->NodeEnd)
    {
        UE_LOG(LogTemp, Error, TEXT("StartNode is nullptr"));
    }
    bActiveWave = true;
    EnemySpawned = 0;
    
    //This starts a timer which spawns Enemy in an interval
    GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::SpawnEnemyInWave, SpawnInterval, true);
    }

    void AWaveManager::SpawnEnemyInWave()
{
    if (EnemySpawned >= NumberOfEnemiesInWave)
    {
        EndWave();
        return;
    }

    if (EnemyClass && GridManager->NodeStart)
    {
        FVector SpawnLocation = GridManager->NodeStart->WorldLocation;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        ARatEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ARatEnemy>(EnemyClass, SpawnLocation, SpawnRotation);

        if (SpawnedEnemy)
        {
            EnemySpawned++;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn RatEnemy!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyClass is not set or NodeStart is wrong"));
    }
}

void AWaveManager::EndWave()
{
    
    GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
    bActiveWave = false;
    EnemySpawned = 0;
    UE_LOG(LogTemp, Warning, TEXT("Wave ended and timer has been reset."));
    }


// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

