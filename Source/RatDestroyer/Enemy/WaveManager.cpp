// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "RatEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Player/PlayerPawn.h"
#include "RatDestroyer/Map/GridManager.h"


// Sets default values
AWaveManager::AWaveManager(): GridManager(nullptr), WaveNumber(1), EnemiesSpawned(0)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    NumberOfEnemiesInWave = 10;
    bActiveWave = false;
    EnemiesSpawned = 0;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    GridManager->NodeStart->WorldLocation;

    EnqueueWave();
   
}

void AWaveManager::EnqueueWave()
{
    int32 BaseEnemyCount = 10;  // Number of enemies in the first wave
    float SpawnInterval = 0.5f; // Spawn interval pr enemy set at 1 second

    // Adds 5 more enemies for each Wave passed
    FMyWave NewWave;
    NewWave.EnemyCount = BaseEnemyCount + (WaveNumber - 1) * 5; 
    NewWave.SpawnInterval = SpawnInterval;

    WaveQueue.Enqueue(NewWave);

    UE_LOG(LogTemp, Log, TEXT("Enqueued Wave %d with %d enemies"), WaveNumber, NewWave.EnemyCount); 
    WaveNumber++; //increases the wave counter
    
}
    void AWaveManager::StartWave() 
    {
    EnemiesAlive.RemoveAll([](ARatEnemy* Enemy) { return !IsValid(Enemy); });

    if (EnemiesAlive.Num() > 0)
    {
        //UE_LOG(LogTemp, Log, TEXT("Seeing if all enemies are dead %d."), WaveNumber);
        return;
    }
    if (WaveQueue.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("Not anymore waves left in queue"));
        return;
    }
    FMyWave NextWave;
    WaveQueue.Dequeue(NextWave);

    NumberOfEnemiesInWave = NextWave.EnemyCount;
    CurrentWaveSpawnInterval = NextWave.SpawnInterval;
    EnemiesSpawned = 0;
    bActiveWave = true;

    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWaveManager::Spawn, CurrentWaveSpawnInterval, true);
    
    }

void AWaveManager::Spawn()
{
    // Just checks for many enemies need to spawn in 
    int32 EnemiesToSpawn = NumberOfEnemiesInWave - EnemiesSpawned;

    // Stops the wave when all enemies are spawned in
    if (EnemiesSpawned >= NumberOfEnemiesInWave)
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle); 
        bActiveWave = false;
        
        UE_LOG(LogTemp, Log, TEXT("Wave %d complete. Enemies spawned: %d"), WaveNumber - 1, EnemiesSpawned);

        // Enqueue the next wave 
        EnqueueWave();
        
        return; 
    }

    // Spawn the Rat on the StartNode
    FVector SpawnLocation = GridManager->NodeStart->WorldLocation;
    ARatEnemy* NewEnemy = GetWorld()->SpawnActor<ARatEnemy>(TheRat, SpawnLocation, FRotator::ZeroRotator);

    if (NewEnemy)
    {
        EnemiesSpawned++;
        EnemiesAlive.Add(NewEnemy);
        
        UE_LOG(LogTemp, Log, TEXT("Enemy Spawned: %d out of %d"), EnemiesSpawned, NumberOfEnemiesInWave);
    }
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Removes dead rats from the list
    EnemiesAlive.RemoveAll([](ARatEnemy* Enemy) { return !IsValid(Enemy); });

    // If there is no wave ongoing and all Rats are dead start timer
    if (EnemiesAlive.Num() == 0 && !bActiveWave && !GetWorld()->GetTimerManager().IsTimerActive(WaveStartTimerHandle))
    {
        // Start 15 second timer when all Rats are dead
        GetWorld()->GetTimerManager().SetTimer(WaveStartTimerHandle, this, &AWaveManager::StartWave, 15.0f, false);

        // Turns on Building
        APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerPawn)
        {
            PlayerPawn->bCanBuild = true;  // Allow building when wave is over
        }
    }
    else if (bActiveWave)
    {
        // If Wave is active turn of building
        APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerPawn)
        {
            PlayerPawn->bCanBuild = false;  // Prevent building when wave is active
        }
    }
}