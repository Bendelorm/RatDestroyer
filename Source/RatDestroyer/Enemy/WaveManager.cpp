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
    StartWave();
    Spawn();
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
    // Checkh for how many Enemies need to spawn
    int32 EnemiesToSpawn = NumberOfEnemiesInWave - EnemiesSpawned;

    // Spawn multiple enemies 
    for (int32 i = 0; i < EnemiesToSpawn; ++i)
    {
        FVector SpawnLocation = GridManager->NodeStart->WorldLocation;
        ARatEnemy* NewEnemy = GetWorld()->SpawnActor<ARatEnemy>(TheRat, SpawnLocation, FRotator::ZeroRotator);

        if (NewEnemy)
        {
            EnemiesSpawned++;
            UE_LOG(LogTemp, Log, TEXT("Enemy Spawned: %d out of %d"), EnemiesSpawned, NumberOfEnemiesInWave);
        }
    }

    // Stop Wave when all enemies are spawnes in
    if (EnemiesSpawned >= NumberOfEnemiesInWave)
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        bActiveWave = false; // innactive wave
    }
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

