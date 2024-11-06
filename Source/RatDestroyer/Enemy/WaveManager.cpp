// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "RatEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Map/GridManager.h"




// Sets default values
AWaveManager::AWaveManager(): GridManager(nullptr)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    NumberOfEnemiesInWave = 5;
    bActiveWave = false;
    EnemiesSpawned = 0;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    GridManager->NodeStart->WorldLocation;
    Spawn();
    StartWave();
}

    void AWaveManager::StartWave() 
    {
    if (NodeStart == nullptr || NodeEnd == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("StartNode is nullptr"));
        return;
    }
    if (bActiveWave) return; //can't start wave if wave is already active

        EnemiesSpawned = 0;
        bActiveWave = true;
        float SpawnInterval = 0.25f;
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWaveManager::Spawn, SpawnInterval, true);
    }

  
void AWaveManager::Spawn()
{
    if (EnemiesSpawned < NumberOfEnemiesInWave)
    {
        FVector SpawnLocation = GridManager->NodeStart->WorldLocation;
      
        ARatEnemy* NewEnemy = GetWorld()->SpawnActor<ARatEnemy>(TheRat, SpawnLocation, FRotator::ZeroRotator);

        if (NewEnemy)
        {
            EnemiesSpawned++;
            UE_LOG(LogTemp, Log, TEXT("Enemy Spawned: %d out of %d"), EnemiesSpawned, NumberOfEnemiesInWave);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn enemy"));
        // Stop the timer once we've reached the target spawn count
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        bActiveWave = false;  // Mark the wave as inactive
    }

}
// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

