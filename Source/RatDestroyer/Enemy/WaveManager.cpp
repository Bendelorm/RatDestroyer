// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "RatDestroyer/Map/GridManager.h"




// Sets default values
AWaveManager::AWaveManager(): GridManager(nullptr), EnemySpawned(0)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bActiveWave = false;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    GridManager->NodeStart->WorldLocation;
}

    void AWaveManager::StartWave() const
    {
    if (!GridManager || !GridManager->NodeStart || !GridManager->NodeEnd)
    {
        UE_LOG(LogTemp, Error, TEXT("StartNode is nullptr"));
        return;
    }
}
    void AWaveManager::SpawnEnemyInWave()
    {
    if (EnemySpawned >= NumberOfEnemiesInWave)
    {
        EndWave();
        return;
    }
        SpawnEnemy(1, TSubclassOf<ARatEnemy>(EnemyClass));
        EnemySpawned++;
    

    if (GridManager && GridManager->NodeStart)
    {
        FVector SpawnLocation = GridManager->NodeStart->WorldLocation;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        // Spawn the enemy
        ARatEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ARatEnemy>(EnemyClass, SpawnLocation, SpawnRotation);
        
        if (SpawnedEnemy)
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawned RatEnemy at: %s"), *SpawnLocation.ToString());
            EnemySpawned++;
        }
    }
    }

    void AWaveManager::EndWave()
    {
        if(GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
        }

        bActiveWave = false;

        EnemySpawned = 0;
    UE_LOG(LogTemp, Warning, TEXT("Wave ended and timer has been reset."));
    }


// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

