// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "RatEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Map/GridManager.h"




// Sets default values
AWaveManager::AWaveManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    NumberOfEnemiesInWave = 10;
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
        EnemiesSpawned = 0;
    }

  
void AWaveManager::Spawn()
{
    for (int32 i= 0 < NumberOfEnemiesInWave; i++;)
        {
    GetWorld()->SpawnActor(TheRat);
    FVector SpawnLocation = GridManager->NodeStart->WorldLocation;
    this->SetActorLocation(SpawnLocation);

        float SpawnInterval = 1.0f;
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AWaveManager::Spawn, SpawnInterval, true);

        
        }
    }


// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

