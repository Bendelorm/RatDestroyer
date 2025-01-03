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
    NumberOfEnemiesInWave = 3; //Number of Enemies in the first wave
    bActiveWave = false; 
    EnemiesSpawned = 0; //Reset Enemy spawn number
    bCleanTowerArray = false;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();

    //Gets reference to GridManager actor that is placed 
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    //Access start location of nodeStart
    GridManager->NodeStart->WorldLocation;

    //Starts the first wave after timer
    EnqueueWave();
   
}

void AWaveManager::EnqueueWave()
{
    int32 BaseEnemyCount = 3;  // Number of enemies in the first wave
    float SpawnInterval = 1.0f; // Spawn interval pr enemy

    // Adds 2 more enemies for each Wave passed
    FMyWave NewWave;
    NewWave.EnemyCount = BaseEnemyCount + ((WaveNumber - 1) * 2); 
    NewWave.SpawnInterval = SpawnInterval;

    //Adds new wave to queue
    WaveQueue.Enqueue(NewWave);
    UE_LOG(LogTemp, Log, TEXT("Enqueued Wave %d with %d enemies"), WaveNumber, NewWave.EnemyCount); 
    WaveNumber++; //increases the wave counter
    
}
    void AWaveManager::StartWave() 
    {
    //Checks if there are any waves left in the queue
    if (WaveQueue.IsEmpty())
    {
        return;
    }
    //Clears spawn timer so it dosent overlap
    GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle); 

    //gets the next wave from the queue
    FMyWave NextWave;
    WaveQueue.Dequeue(NextWave);

    //Wave Parameters
    NumberOfEnemiesInWave = NextWave.EnemyCount;
    CurrentWaveSpawnInterval = NextWave.SpawnInterval;
    EnemiesSpawned = 0;
    bActiveWave = true;
    bCleanTowerArray = false;

    //Starts spawn timer to call the spawn multiple times
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
        GetWorld()->GetTimerManager().SetTimer(WaveStartTimerHandle, this, &AWaveManager::StartWave, 20.0f, false);

        // Turns on Building when wave is done
        APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerPawn)
        {
            PlayerPawn->bCanBuild = true;
            PlayerPawn->bCanUndo = true;
            bCleanTowerArray = true;
        }
    }
    else if (bActiveWave)
    {
        // Turns of building when wave is active
        APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerPawn)
        {
            PlayerPawn->bCanBuild = false;
            PlayerPawn->bCanUndo = false;
        }
    }
}